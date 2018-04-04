#include "PagesGenerator.h"

#include <QDebug>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QThread>
#include <QTime>

#include "ReadingManager.h"
#include "WordsManager.h"
#include "loggerDefine.h"

PagesGenerator::PagesGenerator(QObject *parent)
    : QObject(parent), m_docWidth(600), m_docHeight(600), m_highlightWords(true)
{
    m_doc = std::make_shared<QTextDocument>();
}

PagesGenerator::~PagesGenerator() {}

/**
 * @brief PagesGenerator::setPageSize
 * TODO:
 * @param width
 * @param height
 */
void PagesGenerator::setPageSize(int width, int height)
{
    if ((m_docWidth != width) || (m_docHeight != height))
    {
        WCNT_LOG("Changing page size to ({}, {})", width, height);

        m_docWidth = width;
        m_docHeight = height;

        m_doc->setPageSize(QSize(m_docWidth, m_docHeight));
        m_doc->pageCount();    // NOTE: needed for correct calculating lines per page
    }
}

/**
 * @brief PagesGenerator::setNewHtmlContent
 * [TODO]
 * @param text
 */
void PagesGenerator::setNewHtmlContent(const QString &text)
{
    m_doc.reset(new QTextDocument);
    m_doc.get()->setParent(this);
    m_doc.get()->setDefaultStyleSheet(
        "body {text-align:justify; font-family: \"Heuristica\" ; font-size: 19px; "
        "color: #433B32; } "
        "p { font-family: \"Heuristica\" ; font-size: 19px; color: #433B32; } "
        "a { color: #433B32; }");

    m_doc.get()->setHtml(text);

    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);

    m_doc.get()->setDocumentMargin(0);
    m_doc->setDefaultTextOption(opt);
    m_doc->setPageSize(QSize(m_docWidth, m_docHeight));
    m_doc->pageCount();    // NOTE: needed for correct calculating lines per page

    m_calculatedPages.clear();

    m_htmlBookText = text;
}

void PagesGenerator::setWordsManager(WordsManager *manager) { m_wordsManager = manager; }

void PagesGenerator::setReadingManager(ReadingManager *manager)
{
    m_readingManager = manager;
}

/**
 * @brief PagesGenerator::highlightWordsOnPage
 * [TODO]
 * @param page
 */
void PagesGenerator::highlightWordsOnPage(int page)
{
    if (page >= m_calculatedPages.count() || m_calculatedPages.count() == 0) return;

    QString htmlPageText = m_calculatedPages[page].m_originalHtml;

    QTextDocument bufDoc;
    bufDoc.setHtml(htmlPageText);

    QTextCursor cursor(&bufDoc);
    cursor.setPosition(0, QTextCursor::MoveAnchor);

    while (!cursor.atEnd())
    {
        cursor.select(QTextCursor::WordUnderCursor);
        QString word = cursor.selectedText();

        // for dealing with punctuation
        if (word.length() == 1 && !word[0].isLetter())
        {
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            cursor.select(QTextCursor::WordUnderCursor);
            word = cursor.selectedText();
        }

        // [IMAGES] !!!!
        // "\uFFFC" - image. if was selected image
        if ((word != "") && (word != "\uFFFC"))
        {
            std::string wordStd = word.toLower().toStdString();
            if (!m_wordsManager->isWordKnownOrIgnored(word))    // unknown word
            {
                QTextCharFormat format = cursor.blockCharFormat();

                // QColor unknownColor(210, 77, 87);    // very good red
                QColor unknownColor(220, 20, 60);    // red  #dc143c!!!

                format.setForeground(QBrush(unknownColor));

                // BUG
                format.setFontWeight(QFont::Bold);
                format.setFontPointSize(13.5);    // 18 px

                cursor.mergeCharFormat(format);
            }
        }
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor);
    }

    cursor.select(QTextCursor::Document);
    // m_calculatedPages[page].m_highlightedFragment = cursor.selection();
    m_calculatedPages[page].m_highlightedHtml = cursor.selection().toHtml();
}

/****** PAGE CALCULATING *****/

/**
 *  @brief Calculates new pages from the given position (in percents)
 *
 *  Calculates three pages: previous, current and next pages. We need at least three items
 *  for ListView (QML) in order to allow smooth page scrolling in both directions -
 *  forward and backward.
 *
 *  Gets position in original document from where to start calculating pages. If percents:
 *      - in range [0; 100]: converts percents into position in the book based on the
 *      total character count;
 *      - -1: get start (top left) position of the currently displayed page;
 *      - otherwise (not in range) assume percent = 0.
 *
 *  Then calculates two pages in forward direction (current and next page) starting from
 *  the found position in the book.
 *
 *  Then calculates one page backward (previous page). And updates index of the currently
 *  displayed page to match "middle" of the three calculated pages.
 *
 *  If unknown words in the book should be highlighted then do that.
 *
 *  @param percent - amount percents that represent a certain place in the book. By
 *  default equal to -1 which means that new pages should be re-calculated for the page
 *  currently displayed. Otherwise can be equal to any number in range [0; 100]. If the
 *  provided number is incorrect (below 0 or above 100) the percents are supposed to be
 *  equal 0 (the begining of the book).
 *
 *  @return m_currentIndex - index of the page to display in ListView in QML
 */
int PagesGenerator::calculateNearbyPages(int pos)
{
    WCNT_LOG("Calculating nearby pages for {}...", pos);

    // NOTE: ??? need this or not? MOVE TO SET SIZE
    m_doc.get()->setPageSize(QSize(m_docWidth, m_docHeight));
    m_doc.get()->setDocumentMargin(0);
    m_doc.get()->pageCount();

    // int pos = 0;

    // check and convert percents into position in the book
    m_calculatedPages.clear();

    int forwardPages = 6;
    QStringList addedPages = calculateSeveralPagesForward(pos, forwardPages, false);

    int size = m_calculatedPages.size();

    int backwardPages = 5;
    calculateSeveralPagesBackward(pos, backwardPages, true);

    int index = m_calculatedPages.size() - size;

    // m_currentIndex = addedPages;
    return index;
}

QStringList PagesGenerator::loadBackPages()
{
    QStringList addedPages;
    int page = m_calculatedPages.size() - 1;
    WCNT_LOG("Load back pages for {}...", page);

    bool firstPage = (m_calculatedPages[0].m_startPos == 0);

    if (!firstPage || m_calculatedPages.size() == 0)
    {
        int pos = 0;
        // if (m_calculatedPages.size() > 0)
        pos = m_calculatedPages[0].m_startPos;

        addedPages = calculateSeveralPagesBackward(pos, 5, true);
    }

    return addedPages;
}

/**
 *  @brief Changes the height of the found image in the book in order to fit the page.
 *
 *  Gets information about the image (image format), changes its height and replace old
 *  image with the new one with correct height.
 *
 *  @param cursor - cursor positioned at the image element.
 */
void PagesGenerator::adjustCurrentImageHeight(QTextCursor *cursor)
{
    QTextImageFormat imageFormat = cursor->charFormat().toImageFormat();

    // FIXME : not all images jpeg
    // data:image/jpeg;base64,
    QString img64 = imageFormat.name().remove(0, 23);

    QByteArray by = QByteArray::fromBase64(img64.toLatin1());
    QImage img;

    bool t = img.loadFromData(by, "JPEG");

    // QLabel label(0);
    // label.setPixmap(QPixmap::fromImage(img));

    // https://stackoverflow.com/questions/6912456/how-do-i-save-base64-string-as-png-image-in-qt

    // FUCK YEAAAH!!

    //    int width = label.pixmap()->width();
    //   int height = label.pixmap()->height();

    // int width = 100;
    // int height = 100;

    QPixmap pix = QPixmap::fromImage(img);
    int width = pix.width();
    int height = pix.height();

    // imageFormat.setHeight(height);

    int widthPercents = (width * 100) / m_docWidth;
    int heightPercents = (height * 100) / m_docHeight;

    if (widthPercents > 100 || heightPercents > 100)
    {
        if (heightPercents > widthPercents)
        {
            height = m_docHeight * 0.98;
            imageFormat.setHeight(height);
        }
        else
        {
            width = m_docWidth * 0.98;
            imageFormat.setWidth(width);
        }
    }

    cursor->select(QTextCursor::WordUnderCursor);
    cursor->removeSelectedText();
    cursor->insertImage(imageFormat);
}

/**
 *  @brief Generates new page filling it with the book content moving down line by line.
 *
 *  Creates an empty document for a page and fills it line by line with the original book
 *  content until the new page height won't become greater than it should be (the page is
 *  fiiled). Than it returns one line up so the new page height is correct and the content
 * can fit into one page.
 *
 *  After generating the page saves its content and positions in the original book (top
 *  left and bottom right character positions).
 *
 *  @param pageCursor - cursour positioned at the start of some line in the book. That
 *  line will become a top line of the newly generated page.
 */
QString PagesGenerator::calculatePageFromTop(QTextCursor &pageCursor)
{
    QTime myTimer;
    myTimer.start();

    QString pageHtml;

    // NOTE: not sure we need this
    // some tex document settings
    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);

    QTextDocument bufDoc;
    bufDoc.setDefaultTextOption(opt);
    bufDoc.setDocumentMargin(0);
    bufDoc.setPageSize(QSize(m_docWidth, m_docHeight));

    while ((bufDoc.size().height() < m_docHeight) && (!pageCursor.atEnd()))
    {
        QTime myTimer2;
        myTimer2.start();

        int startPagePos = pageCursor.anchor();
        int currentPos = pageCursor.position();

        // WARNING: heavy op
        if (pageCursor.charFormat().isImageFormat())
        {
            adjustCurrentImageHeight(&pageCursor);
        }

        pageCursor.setPosition(startPagePos, QTextCursor::MoveAnchor);
        pageCursor.setPosition(currentPos, QTextCursor::KeepAnchor);
        pageCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

        bufDoc.setHtml(pageCursor.selection().toHtml());
        bufDoc.pageCount();    // !!!

        pageCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
        pageCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    }

    if (!pageCursor.atEnd())
        // if the height of the page is greater than it should be - remove last line
        if (bufDoc.size().height() >= m_docHeight)
        {
            // FIXME: remember percents of filling the page (remove 1, 2, 3 last lines)
            pageCursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, 2);
            pageCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

            bufDoc.setHtml(pageCursor.selection().toHtml());
        }
    // save calculated page
    SinglePage bufPage;
    bufPage.m_startPos = pageCursor.anchor();
    bufPage.m_endPos = pageCursor.position();

    bufPage.m_originalHtml = pageCursor.selection().toHtml();
    bufPage.m_highlightedHtml = pageCursor.selection().toHtml();

    m_calculatedPages.push_back(bufPage);

    // pageCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
    pageCursor.movePosition(QTextCursor::NoMove, QTextCursor::MoveAnchor);
    if (m_highlightWords)
    {
        int currentPage = m_calculatedPages.size() - 1;
        highlightWordsOnPage(currentPage);

        // bufDoc.setHtml(pageCursor.selection().toHtml());
        bufDoc.setHtml(m_calculatedPages[currentPage].m_highlightedHtml);

        int deltaPos = adjustHighlightedPageHeight(currentPage);
        int newEndPos = bufPage.m_endPos - deltaPos;
        pageCursor.setPosition(newEndPos + 1, QTextCursor::MoveAnchor);

        // pageCursor.setPosition(newEndPos, QTextCursor::MoveAnchor);
        pageCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);

        bufPage.m_endPos = newEndPos;
        m_calculatedPages[currentPage].m_endPos = newEndPos;
    }

    int nMilliseconds = myTimer.elapsed();
    WCNT_LOG(
        "[CALCULATING PAGES] FORWARD calculating and highlighting {} page took {} ms.",
        m_calculatedPages.size() - 1, nMilliseconds);
    // FIXME : CHECK FOR size 0
    return m_calculatedPages.last().m_highlightedHtml;
}

/**
 *  @brief Calculate pages from the given position in forward direction.
 *
 *  Calculates given number of pages starting from the line with the given postion in the
 *  opened book. Generated pages contain HTML-formated strings and are used in ListView to
 *  display pages.
 *
 *  @param startGlobalPos - start position from where to start page generation. If
 *  position is out of range, then supposed to be 0.
 *
 *  @param pagesToCalculate - number of pages to calculate.
 *      @c -1 - (default) calculate all pages till the end of the book
 *      @c positive number - calculate given number of pages
 *      @c wrong number - is equal to -1 (calculate till the begining)
 *
 *  @param keepOldPages - should the old pages be saved
 *      @c true - keep already calculated pages;
 *      @c false - remove already calculated pages
 */
QStringList PagesGenerator::calculateSeveralPagesForward(int startGlobalPos,
                                                         int pagesToCalculate,
                                                         bool keepOldPages)
{
    WCNT_LOG("[CALCULATING PAGES] Calculate {} pages forward from {} pos.",
             pagesToCalculate, startGlobalPos);

    QTime myTimer;
    myTimer.start();

    QStringList addedPages;

    if (!keepOldPages) m_calculatedPages.clear();

    // !!! SHOULD I CHECK FOR m_doc to exist ???
    QTextCursor pageCursor(m_doc.get());

    pageCursor.setPosition(startGlobalPos, QTextCursor::MoveAnchor);

    // check input params
    bool calculateAllPages = false;
    if (startGlobalPos < 0 ||
        startGlobalPos >
            m_doc.get()->characterCount())    // WARNING: CHARS COUNT INTO VAR
        startGlobalPos = 0;
    if (pagesToCalculate < 0) calculateAllPages = true;

    int page = 0;
    while (!pageCursor.atEnd() && ((page < pagesToCalculate) || calculateAllPages))
    {
        QString pageHtml = calculatePageFromTop(pageCursor);
        addedPages << pageHtml;
        emit m_readingManager->addForwardPage(pageHtml);
        page++;
    }

    int nMilliseconds = myTimer.elapsed();
    WCNT_LOG("[CALCULATING PAGES] ...finished calculating (forward). Time: {} ms.",
             nMilliseconds);

    return addedPages;
}

/**
 *  @brief Generates new page filling it with the book content moving up line by line.
 *
 *  Creates an empty document for a page and fills it line by line with the original book
 *  content until the new page height won't become greater than it should be (the page is
 *  fiiled). Than it returns one line down so the new page height is correct and the
 * content can fit into one page.
 *
 *  After generating the page saves its content and positions in the original book (top
 *  left and bottom right character positions).
 *
 *  @param pageCursor - cursour positioned at the end of some line in the book. That
 *  line will become the bottom line of the newly generated page.
 *
 *  @param wasFirstLine - whether the very first line of the book was encountered.
 */
QString PagesGenerator::calculatePageFromBottom(QTextCursor &pageCursor,
                                                bool &wasFirstLine)
{
    QString pageHtml;

    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);

    QTextDocument bufDoc;
    bufDoc.setDefaultTextOption(opt);
    bufDoc.setDocumentMargin(0);

    bufDoc.setPageSize(QSize(m_docWidth, m_docHeight));
    bufDoc.pageCount();    // !!!

    m_doc.get()->pageCount();    // !!!
    while ((bufDoc.size().height() < m_docHeight) && (!wasFirstLine))
    {
        int startPagePos = pageCursor.anchor();
        int currentPos = pageCursor.position();

        if (pageCursor.atStart())
        {
            wasFirstLine = true;
        }
        // pageCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
        // pageCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
        if (pageCursor.charFormat().isImageFormat())
        {
            adjustCurrentImageHeight(&pageCursor);
            m_doc.get()->pageCount();
            pageCursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        }
        else
        {
            pageCursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
            pageCursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        }

        // pageCursor.setPosition(startPagePos, QTextCursor::MoveAnchor);
        // pageCursor.setPosition(currentPos, QTextCursor::KeepAnchor);

        bufDoc.setHtml(pageCursor.selection().toHtml());
        bufDoc.pageCount();    // !!!
    }

    if (bufDoc.size().height() >= m_docHeight)
    {
        pageCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 1);
        pageCursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);

        bufDoc.setHtml(pageCursor.selection().toHtml());
        bufDoc.pageCount();
        if (wasFirstLine) wasFirstLine = false;
    }

    SinglePage bufPage;
    bufPage.m_originalHtml = pageCursor.selection().toHtml();
    bufPage.m_highlightedHtml = pageCursor.selection().toHtml();

    bufPage.m_startPos = pageCursor.position();
    bufPage.m_endPos = pageCursor.anchor();

    m_calculatedPages.insert(0, bufPage);

    pageCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
    if (m_highlightWords)
    {
        highlightWordsOnPage(0);
        bufDoc.setHtml(m_calculatedPages[0].m_highlightedHtml);

        int deltaPos = adjustHighlightedPageHeightFromBottom(0);
        int newStartPos = bufPage.m_startPos + deltaPos;

        pageCursor.setPosition(newStartPos - 1, QTextCursor::MoveAnchor);
        m_calculatedPages[0].m_startPos = newStartPos;

        if (pageCursor.atStart()) wasFirstLine = true;

        WCNT_LOG("BACKWARD calculating and highlighting {} page took ? ms.",
                 m_calculatedPages.size() - 1);
    }
    return m_calculatedPages.first().m_highlightedHtml;
}

/**
 *  @brief Calculates pages from the given position in backward direction.
 *
 *  Calculates given number of pages from the given postion in the opened book. Generated
 *  pages contain HTML-formated strings and are used in ListView to display pages.
 *
 *  @param startGlobalPos - cursor position to display. If position is out of range, then
 *  supposed to be 0.
 *
 *  @param pagesToCalculate - number of pages to calculate.
 *      @c -1 - (default) calculate all pages till the end of the book
 *      @c positive number - calculate given number of pages
 *      @c wrong number - is equal to -1 (calculate till the begining)
 *
 *  @param keepOldPages - should the old pages be saved
 *      @c true - keep already calculated pages;
 *      @c false - remove already calculated pages
 */
QStringList PagesGenerator::calculateSeveralPagesBackward(int startGlobalPos,
                                                          int pagesToCalculate,
                                                          bool keepOldPages)
{
    QStringList addedPages;

    if (!keepOldPages)
    {
        m_calculatedPages.clear();
    }

    // check input params
    if (startGlobalPos > m_doc.get()->characterCount())
    {
        startGlobalPos = m_doc.get()->characterCount() - 1;
        pagesToCalculate = -1;
    }
    if (startGlobalPos == 0) return QStringList();
    bool calculateToBegining = false;
    if (pagesToCalculate < 0) calculateToBegining = true;

    WCNT_LOG("[CALCULATING PAGES] Calculate {} pages backward from {} pos.",
             pagesToCalculate, startGlobalPos);

    QTime myTimer;
    myTimer.start();

    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);
    m_doc.get()->setDefaultTextOption(opt);
    m_doc.get()->setPageSize(QSize(m_docWidth, m_docHeight));
    m_doc.get()->pageCount();    // OOOOOH I LOVE YOU!!!!!!!!!!!!!111111111111111

    QTextCursor pageCursor(m_doc.get());
    pageCursor.setPosition(startGlobalPos, QTextCursor::MoveAnchor);
    pageCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);

    int page = 0;
    bool wasFirstLine = false;
    while (!wasFirstLine && ((page < pagesToCalculate) || calculateToBegining))
    {
        QString pageHtml = calculatePageFromBottom(pageCursor, wasFirstLine);
        addedPages << pageHtml;
        emit m_readingManager->addBackwardPage(pageHtml);
        page++;
    }

    int nMilliseconds = myTimer.elapsed();
    WCNT_LOG("[CALCULATING PAGES] ...finished calculating (backward). Time: {} ms.",
             nMilliseconds);

    return addedPages;
}

int PagesGenerator::adjustHighlightedPageHeight(int page)
{
    QString htmlPageText = m_calculatedPages[page].m_highlightedHtml;

    QTextDocument bufDoc;
    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);    // !!! WARNING : othervise not working

    bufDoc.setDefaultTextOption(opt);
    bufDoc.setDocumentMargin(0);

    bufDoc.setPageSize(QSize(m_docWidth, m_docHeight));
    bufDoc.setHtml(htmlPageText);
    bufDoc.pageCount();

    QTextCursor cursor(&bufDoc);
    // cursor.setPosition(0, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    int endPos = cursor.position();

    while ((bufDoc.size().height() >= m_docHeight) && (!cursor.atStart()))
    {
        cursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        bufDoc.pageCount();
    }

    m_calculatedPages[page].m_highlightedHtml = bufDoc.toHtml();

    int deltaPos = endPos - cursor.position();
    return deltaPos;
}

int PagesGenerator::adjustHighlightedPageHeightFromBottom(int page)
{
    QString htmlPageText = m_calculatedPages[page].m_highlightedHtml;

    QTextDocument bufDoc;
    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);    // !!! WARNING : othervise not working

    bufDoc.setDefaultTextOption(opt);
    bufDoc.setDocumentMargin(0);

    bufDoc.setPageSize(QSize(m_docWidth, m_docHeight));
    bufDoc.setHtml(htmlPageText);
    bufDoc.pageCount();

    int deltaPos = 0;
    QTextCursor cursor(&bufDoc);
    // cursor.setPosition(0, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    int endPos = cursor.position();

    while ((bufDoc.size().height() >= m_docHeight) && (!cursor.atEnd()))
    {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        deltaPos += cursor.position();
        cursor.removeSelectedText();
        bufDoc.pageCount();
    }

    m_calculatedPages[page].m_highlightedHtml = bufDoc.toHtml();
    return deltaPos;
}

/**
 *  @brief Calculate pages in the scrolling direction.
 *
 *  Scrolling direction is determined as difference of old and new index.
 *  TODO
 *
 *  @param page - index of the new page
 *  @return page - index of the page to display in ListView in QML
 */
QStringList PagesGenerator::loadNextPages()
{
    QStringList addedPages;
    int page = m_calculatedPages.size() - 1;
    WCNT_LOG("[CALCULATING PAGES] Load next pages for {}...", page);

    bool lastPage =
        (m_calculatedPages.size() == 0) ||
        (m_calculatedPages.last().m_endPos == m_doc.get()->characterCount() - 1);

    if (!lastPage || m_calculatedPages.size() == 0)
    {
        int pos = 0;
        if (m_calculatedPages.size() > 0) pos = m_calculatedPages.last().m_endPos + 1;
        addedPages = calculateSeveralPagesForward(pos, 5, true);
    }
    return addedPages;
}

void PagesGenerator::resetAll() { m_calculatedPages.clear(); }

/***** GETTERS *****/

int PagesGenerator::foundPagesCount() { return m_calculatedPages.size(); }

int PagesGenerator::getCalculatedPageStartPos(int index)
{
    if (m_calculatedPages.size() > index && index >= 0)
        return m_calculatedPages[index].m_startPos;
    else
        return 0;
}

bool PagesGenerator::hasCalculatedPages() { return m_calculatedPages.size() > 0; }

int PagesGenerator::getCalculatedPageWithPosition(int pos)
{
    int index = -1;
    for (int i = 0; i < m_calculatedPages.size(); i++)
    {
        SinglePage page = m_calculatedPages[i];
        if (pos >= page.m_startPos && pos <= page.m_endPos)
        {
            index = i;
            return index;
        }
    }
    return index;
}

/**
 * @brief PagesGenerator::getHighlightedTextFragments
 * [TODO]
 * @return
 */
QStringList PagesGenerator::getHighlightedTextFragments() const
{
    QStringList newList;
    for (const SinglePage &textFrag : m_calculatedPages)
        newList << textFrag.m_highlightedHtml;

    return newList;
}

/**
 * @brief PagesGenerator::getOriginalTextFragments
 * [TODO]
 * @return
 */
QStringList PagesGenerator::getOriginalTextFragments() const
{
    QStringList newList;
    for (const SinglePage &textFrag : m_calculatedPages)
        newList << textFrag.m_highlightedHtml;

    return newList;
}

QString PagesGenerator::getPlainText() const { return m_doc->toPlainText(); }
