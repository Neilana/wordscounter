#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QTextStream>
#include <QThread>
#include <QUrl>
#include <QXmlQuery>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QXmlStreamReader>

#include "ReadingManager.h"
#include "loggerDefine.h"

ReadingManager::ReadingManager(QObject *parent)
    : QObject(parent), m_highlightUnknownWords(true), m_currentIndex(0)
{
    m_loadingContentOrSize = false;
    m_timeoutOn = true;
}

ReadingManager::~ReadingManager()
{
    if (m_pagesGenerator != NULL) delete m_pagesGenerator;

    if (m_timer != NULL) delete m_timer;
}

void ReadingManager::setWordsHighlighting(bool value) { m_highlightUnknownWords = value; }

int ReadingManager::getCurrentIndex() { return m_currentIndex; }

QStringList ReadingManager::getContentStringList()
{
    QStringList newList;
    for (Section &section : m_sections)
    {
        int pos = section.m_position;
        int percents = positionToPercents(pos);
        newList << "[" + QString::number(percents) + "%] " + section.m_title;
    }

    return newList;
}

int ReadingManager::positionToPercents(int pos)
{
    // WARNING FIXME
    if (m_pagesGenerator->m_doc)
        return (pos * 100) / m_pagesGenerator->m_doc->toPlainText().length();
    else
        return 0;
}

/**
 *  @brief Opens book by its filename
 *
 *  If the fileName is correct and file was opened successfully checks the file extension.
 *  If the file is:
 *      - fb2: then converts it to HTML-string by applying an xsl-stylesheet on it;
 *      - txt: also converts it to HTML and applies some basic style options.
 *
 *  Loads generated HTML-string into book content object and splits it into pages.
 *  Loads splited pages (as HTML-strings) into QML model.
 *
 * @param fileName - full path to the book to opem (with  *.txt or *.fb2 extension)
 */
void ReadingManager::openBook(QString fileName)
{
    m_timer->stop();

    WCNT_LOG("Opening book: {}", fileName.toStdString());
    emit changeProgramStatus("Opening book...");

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QFileInfo fileInfo(file);
    QString extension = fileInfo.suffix();

    QString htmlBookText;
    // *.fb2
    if (extension == "fb2")
    {
        QXmlQuery query(QXmlQuery::XSLT20);
        // CHECK FOR fb2???
        query.setFocus(&file);
        query.setQuery(QUrl("qrc:/myStylesheet_sections.xsl"));
        query.evaluateTo(&htmlBookText);
    }
    // *.txt
    else
    {
        QTextStream in(&file);
        htmlBookText = "<div align=\"justify\">";

        while (!in.atEnd())
        {
            htmlBookText += "<p style='margin-top:2px; margin-bottom:2px; '> " +
                            in.readLine() + "</p>";
        }

        htmlBookText += "</div>";
    }

    m_fileName = fileName;

    m_pagesGenerator->resetAll();
    m_pagesGenerator->setNewHtmlContent(htmlBookText);

    // generate table of contents fb2
    emit changeProgramStatus("Generating table of contents...");

    m_sections.clear();
    m_notes.clear();

    emit startedLoadingByPercents();

    emit changeProgramStatus("Loading pages...");
    QStringList addedPages = m_pagesGenerator->loadNextPages();    // 1

    emit finishedCalculating();
    emit changeProgramStatus("");

    m_timer->start(1000);
}

/**
 *  @brief Set width and height of book page
 *
 *  Updates width and height of book page in the book content object. Width and height
 *  should be equal to the dimensions of book page window in QML-form. The method is
 *  usually called:
 *      - at the begining of the program (when the page size wasn't yet defined);
 *      - when the reading window with is resized by hand;
 *      - when some moving operations are applied to the new word window
 *      (e.g. snapping/unapping, sliding/unsliding).
 *
 *  After setting new size updates content of the page. Recalculates the content of the
 *  page, and also one page back, one page forward (in order to allow smooth scrolling
 *  with ListView in QML we need 3 items).
 *
 *  Then updates QML page model and replaces old HTML-pages with the new ones.

 *  Depending on the new page size new page index in ListView can change, so we pass that
 *  new index up to the core where the method was called.
 *
 *  @param width - new width (idealy it should be equal to page width in QML-form)
 *  @param height -  new height (idealy it should be equal to page height in QML-form)
 *  @return newIndex - index of the current page in calculated string list
 */
void ReadingManager::setPageSize(int width, int height, int index)
{
    m_loadingContentOrSize = true;
    emit clearBookModel();
    int pos = m_pagesGenerator->getCalculatedPageStartPos(index);
    m_pagesGenerator->resetAll();
    m_pagesGenerator->setPageSize(width, height);

    m_pagesGenerator->calculateNearbyPages(pos);
    m_loadingContentOrSize = false;

    emit finishedCalculating();
}

/**
 *  @brief Calculate next pages in the scrolling direction
 *
 *  [TODO]
 *
 *  Then updates QML page model and replaces old HTML-pages with the new ones.

 *  Depending on the new page size new page index in ListView can change, so we pass that
 *  new index up to the core where the method was called.
 *
 *  @param pageIndex - index of the new page
 *  @return newIndex - index of the current page in calculated string list
 */
void ReadingManager::loadNextPages()
{
    if (!m_fileName.isEmpty()) QStringList addedPages = m_pagesGenerator->loadNextPages();

    emit finishedCalculating();
}

void ReadingManager::loadBackPages()
{
    if (!m_fileName.isEmpty()) QStringList addedPages = m_pagesGenerator->loadBackPages();

    emit finishedCalculating();
}

void ReadingManager::setWordsManager(WordsManager *manager)
{
    m_wordsManager = manager;
    m_pagesGenerator->setWordsManager(manager);
}

void ReadingManager::onTimeout()
{
    if (m_fileName != "" && !m_loadingContentOrSize && m_timeoutOn)
    {
        loadNextPages();
        loadBackPages();
    }
}

void ReadingManager::loadFromContentsTable(int sectionIndex)
{
    m_timer->stop();

    m_loadingContentOrSize = true;
    m_timeoutOn = false;

    int pos = m_sections[sectionIndex].m_position;

    int index = m_pagesGenerator->getCalculatedPageWithPosition(pos);
    if (index > -1)
    {
        WCNT_LOG("[GO TO CONTENTS] Page was calculated. Going to {} page.", index);
        emit goToPage(index);
    }
    else
    {
        WCNT_LOG(
            "[GO TO CONTENTS] Page was not calculated. Start calculation for {} "
            "position.",
            pos);
        emit startedLoadingByPercents();
        index = m_pagesGenerator->calculateNearbyPages(pos);
    }

    m_loadingContentOrSize = false;
    m_timeoutOn = true;

    emit finishedCalculating();
    m_timer->start(1000);
}

QSet<QString> ReadingManager::readLinksIdsFromFb2()
{
    QSet<QString> linksIds;
    QString htmlText = m_pagesGenerator->m_doc->toHtml();
    QDomDocument doc;
    doc.setContent(htmlText);

    // FIXME : not body=notes!
    QDomNodeList as = doc.elementsByTagName("a");
    for (int i = 0; i < as.size(); i++)
    {
        QDomNode aNode = as.item(i);
        QString aName = aNode.attributes().namedItem("href").nodeValue();
        if (aName[0] == '#')    // =id
        {
            QString id = aName.remove(0, 1);
            linksIds.insert(id);
        }
    }
    return linksIds;
}

void ReadingManager::readLinksPositions(QSet<QString> ids) {}

void ReadingManager::loadPageByPercents(QString percentsStr)
{
    m_timer->stop();

    percentsStr.chop(1);    // delete "%"
    float percents = percentsStr.toFloat();

    emit startedLoadingByPercents();

    int pos = (percents * m_pagesGenerator->m_doc->toPlainText().length()) / 100;
    int index = m_pagesGenerator->getCalculatedPageWithPosition(pos);

    if (index > -1)
    {
        emit goToPage(index);
    }
    else
    {
        int index = m_pagesGenerator->calculateNearbyPages(pos);
    }

    emit finishedCalculating();
    m_timer->start(1000);
}

long ReadingManager::countUnknownWords()
{
    long result = 0;

    QString text = m_pagesGenerator->getPlainText();
    QStringList wordList = text.split(QRegExp("[\\.,\\s!;?:\"()']+"));
    wordList = wordList.filter(QRegExp("[a-zA-Z]+"));

    QSet<QString> set;
    for (auto word : wordList)
    {
        bool knownWord = m_wordsManager->isWordKnownOrIgnored(word.toLower());
        if (!knownWord) set.insert(word.toLower());
    }

    result = set.size();
    return result;
}

/* THREADS */
void ReadingManager::init()
{
    m_pagesGenerator = new PagesGenerator;
    m_pagesGenerator->setReadingManager(this);

    m_timer = new QTimer;
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void ReadingManager::closeOpenedBookAtPage(int index)
{
    m_timer->stop();

    if (!m_fileName.isEmpty())
    {
        int pos = m_pagesGenerator->getCalculatedPageStartPos(index);
        emit saveCurrentPositionForOpenedBook(pos);
    }
    m_fileName = "";
    m_pagesGenerator->resetAll();
}

void ReadingManager::setNotes(QMap<QString, QString> notes)
{
    m_notes = notes;
    emit updateNotesSignal(m_notes);
}

void ReadingManager::setSections(QVector<Section> sections)
{
    m_sections = sections;
    QStringList tableOfContent = getContentStringList();
    emit updateTableOfContent(tableOfContent);
}
