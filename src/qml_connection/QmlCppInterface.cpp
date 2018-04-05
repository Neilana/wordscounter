#include <QDomDocument>

#include "Core.h"
#include "QmlCppInterface.h"

#include "loggerDefine.h"

QmlCppInterface::QmlCppInterface(QObject* parent)
    : QObject(parent), m_currentPageIndex(0), m_percents(0), m_bookOpened(false)
{
    srand(time(NULL));
    fillHelloAnimalsVector();

    m_timer = new QTimer;
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(clearProgramStatus()));
}

QmlCppInterface::~QmlCppInterface()
{
    if (m_timer != NULL) delete m_timer;
}

void QmlCppInterface::setCore(Core* core) { m_core = core; }

void QmlCppInterface::setPageIndex(int i) { m_currentPageIndex = i; }

void QmlCppInterface::openBookById(int id)
{
    if (!m_core->m_calculating)    // ???
    {
        m_bookOpened = false;
        emit bookOpenedChanged();

        m_core->m_calculating = true;
        m_core->openBook(id);

        m_currentPageIndex = 0;
        emit pageIndexChanged();
    }
}

void QmlCppInterface::deleteBookById(int id) { emit deleteBookByIdSignal(id); }

void QmlCppInterface::setPageSize(int width, int height)
{
    if (!m_core->m_calculating)
    {
        int index = m_currentPageIndex;
        m_currentPageIndex = 0;
        emit pageIndexChanged();
        m_core->clearBookModel();
        emit setPageSizeSignal(width, height, index);
    }
}

QString QmlCppInterface::countAllUnknownWordsByBookId(int id)
{
    return m_core->countAllUnknownWordsByBookId(id);
}

int QmlCppInterface::getPageIndex() { return m_currentPageIndex; }

int QmlCppInterface::getPercents() { return m_percents; }

QString QmlCppInterface::getOpenedNote() const { return m_openedNote; }

QString QmlCppInterface::getProgramStatus() const { return m_programStatus; }

QString QmlCppInterface::getSearchedWord() const { return m_searchedWord; }

QString QmlCppInterface::getHelloAnimalImagePath(int id) const
{
    if (id >= 0 && id <= m_helloAnimals.size() - 1)
        return m_helloAnimals[id].m_fileName;
    else
        return {};
}

QString QmlCppInterface::getHelloAnimalName(int id) const
{
    if (id >= 0 && id <= m_helloAnimals.size() - 1)
        return m_helloAnimals[id].m_name;
    else
        return {};
}

int QmlCppInterface::isBookOpened() { return m_bookOpened; }

void QmlCppInterface::addNewBookByFilename(QUrl fileUrl)
{
    QString fileName = fileUrl.toLocalFile();
    emit addNewBookByFilenameSignal(fileName);
}

bool QmlCppInterface::isLink(const QString& htmlStr)
{
    QDomDocument doc;
    doc.setContent(htmlStr);
    // FIXME: body notes ??? or all links
    QDomNode linkNode = doc.elementsByTagName("a").at(0);
    bool wasLink = false;
    if (!linkNode.isNull())
    {
        QString linkName = linkNode.attributes().namedItem("href").nodeValue();
        // open it
        if (linkName != "") wasLink = true;
    }
    return wasLink;
}

bool QmlCppInterface::isNote(const QString& htmlStr)
{
    QDomDocument doc;
    doc.setContent(htmlStr);
    // FIXME: body notes ??? or all links
    QDomNode linkNode = doc.elementsByTagName("a").at(0);
    bool wasNote = false;
    if (!linkNode.isNull())
    {
        QString linkName = linkNode.attributes().namedItem("href").nodeValue();
        // open it
        if (linkName[0] == '#')    // id
        {
            linkName.remove(0, 1);
            if (m_notes.count(linkName) > 0)
            {
                m_openedNote = m_notes[linkName];
                emit openedNoteChanged();
                wasNote = true;
            }
        }
    }
    return wasNote;
}

QString QmlCppInterface::extractLink(const QString& htmlStr)
{
    QString linkName;
    QDomDocument doc;
    doc.setContent(htmlStr);
    // FIXME: body notes ??? or all links
    QDomNode linkNode = doc.elementsByTagName("a").at(0);

    if (!linkNode.isNull())
    {
        linkName = linkNode.attributes().namedItem("href").nodeValue();
    }
    return linkName;
}

void QmlCppInterface::wordClicked(const QString& word)
{
    if (word != "") m_core->wordClicked(word);
}

void QmlCppInterface::addWord(const QString& word, const QString& definition,
                              const QString& example)
{
    WordItem bufWord(word, "", definition, example);
    m_core->addWord(bufWord);
}

void QmlCppInterface::closeProgram() { m_core->closeProgram(); }

bool QmlCppInterface::isImage(const QString& htmlStr)
{
    QDomDocument doc;
    doc.setContent(htmlStr);

    QDomNode imageNode = doc.elementsByTagName("img").at(0);
    bool wasImage = false;

    if (!imageNode.isNull()) wasImage = true;

    return wasImage;
}

void QmlCppInterface::contentClicked(int index)
{
    // FIXMEEE
    //     if (!m_core->m_waitLoadingByPercents)
    {
        WCNT_LOG("Contents clicked: {}!", index);

        m_core->m_waitLoadingByPercents = true;
        emit loadFromContentsTable(index);
    }
}

void QmlCppInterface::searchedWordClicked(int index)
{
    // qDebug() << "searchedWord: " << index;
}

int QmlCppInterface::generateHelloAnimalNumber()
{
    int animalId = rand() % (m_helloAnimals.size() - 1);
    return animalId;
}

void QmlCppInterface::updateNotes(QMap<QString, QString> notes) { m_notes = notes; }

void QmlCppInterface::changeProgramStatus(QString status)
{
    m_programStatus = status;
    emit programStatusChanged();
    if (status != "")
        m_timer->start(2300);
    else
        m_timer->stop();
}

void QmlCppInterface::updateSearchedWord(QString word)
{
    m_searchedWord = word;
    emit searchedWordChanged();
}

void QmlCppInterface::clearProgramStatus() { changeProgramStatus(""); }

void QmlCppInterface::fillHelloAnimalsVector()
{
    m_helloAnimals.push_back(HelloAnimal{"narwhal.png", "narwhal"});
    m_helloAnimals.push_back(HelloAnimal{"octopus_2.png", "ninja octopus"});
    m_helloAnimals.push_back(HelloAnimal{"unicorn.png", "unicorn"});
    m_helloAnimals.push_back(HelloAnimal{"bee.png", "bee"});
    m_helloAnimals.push_back(HelloAnimal{"crab.png", "crab"});
    m_helloAnimals.push_back(HelloAnimal{"dragon_1.png", "dragon"});
    m_helloAnimals.push_back(HelloAnimal{"dragon_2.png", "dragon"});
    m_helloAnimals.push_back(HelloAnimal{"fish.png", "fish"});
    m_helloAnimals.push_back(HelloAnimal{"turtle.png", "turtle"});

    //    m_helloAnimals.push_back(HelloAnimal{"deer.png", "deer"});
    //    m_helloAnimals.push_back(HelloAnimal{"octopus_1.png", "octopus"});
}

void QmlCppInterface::setBookOpened(bool value)
{
    m_bookOpened = value;
    emit bookOpenedChanged();
}

void QmlCppInterface::loadPageByPercents(QString percentsStr)
{
    if (!m_core->m_waitLoadingByPercents)
    {
        m_core->clearBookModel();
        m_core->m_waitLoadingByPercents = true;
        emit loadPageByPercentsSignal(percentsStr);
    }
}
