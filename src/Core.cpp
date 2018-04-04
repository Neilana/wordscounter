/// @file Core.cpp

#include <QQmlContext>

#include "Core.h"
#include "LibraryBook.h"
#include "loggerDefine.h"

Core::Core(QObject *parent)
    : QObject(parent), m_calculating(false), m_workingThread(new QThread)
{
    registerMetaTypes();
    setUpConnections();
    setUpModels();
    m_waitLoadingByPercents = false;
}

Core::~Core()
{
    // FIXME: THREADS DESTROYED
    // if (m_workingThread != NULL) m_workingThread->deleteLater();
}

void Core::setUpModels()
{
    QSqlDatabase db = LibraryDatabase::getInstance();

    m_libraryModel = std::make_shared<MySqlQueryModel>();
    m_libraryModel->setQuery("SELECT * FROM Books", db);
}

void Core::registerMetaTypes()
{
    qRegisterMetaType<QMap<QString, QString>>();

    qRegisterMetaType<Section>("Section");
    qRegisterMetaType<QVector<Section>>("QVector<Section>");

    qRegisterMetaType<WordItem>("WordItem");
    qRegisterMetaType<QVector<WordItem>>("QVector<WordItem>");
}

// network
void Core::setUpNetworkConnections()
{
    QObject::connect(&m_networkManager,
                     SIGNAL(recievedEntryResultSignal(QVector<WordItem>)), this,
                     SLOT(recievedEntryResult(QVector<WordItem>)));

    QObject::connect(&m_networkManager, SIGNAL(recievedSearchResultSignal(QString)),
                     &m_qmlCppInterface, SLOT(updateSearchedWord(QString)));
}

// QmlCppInterface <--> ReadingManager
void Core::setUpGuiWorkerConnections()
{
    QObject::connect(&m_qmlCppInterface, SIGNAL(loadFromContentsTable(int)),
                     &m_readingManager, SLOT(loadFromContentsTable(int)));

    QObject::connect(&m_libraryManager, SIGNAL(changeProgramStatus(QString)),
                     &m_qmlCppInterface, SLOT(changeProgramStatus(QString)));

    QObject::connect(&m_readingManager, SIGNAL(changeProgramStatus(QString)),
                     &m_qmlCppInterface, SLOT(changeProgramStatus(QString)));

    QObject::connect(&m_qmlCppInterface, SIGNAL(setPageSizeSignal(int, int, int)),
                     &m_readingManager, SLOT(setPageSize(int, int, int)));

    QObject::connect(&m_qmlCppInterface, SIGNAL(loadPageByPercentsSignal(QString)),
                     &m_readingManager, SLOT(loadPageByPercents(QString)));

    QObject::connect(&m_readingManager, SIGNAL(updateNotesSignal(QMap<QString, QString>)),
                     &m_qmlCppInterface, SLOT(updateNotes(QMap<QString, QString>)));
}

void Core::setUpCoreWorkerConnections()
{
    // Core <--> ReadingManager
    QObject::connect(&m_readingManager, SIGNAL(startedLoadingByPercents()), this,
                     SLOT(startedLoadingByPercents()));

    QObject::connect(&m_readingManager, SIGNAL(goToPage(int)), this, SLOT(goToPage(int)));

    QObject::connect(&m_readingManager, SIGNAL(updateTableOfContent(QStringList)), this,
                     SLOT(updateTableOfContent(QStringList)));

    QObject::connect(&m_readingManager, SIGNAL(updateForwardPages(QStringList)), this,
                     SLOT(updateForwardPages(QStringList)));

    QObject::connect(&m_readingManager, SIGNAL(updateBackwardPages(QStringList)), this,
                     SLOT(updateBackwardPages(QStringList)));

    QObject::connect(&m_readingManager, SIGNAL(addForwardPage(QString)), this,
                     SLOT(addForwardPage(QString)));

    QObject::connect(&m_readingManager, SIGNAL(addBackwardPage(QString)), this,
                     SLOT(addBackwardPage(QString)));

    QObject::connect(&m_readingManager, SIGNAL(reloadPagesAndIndex(QStringList, int)),
                     this, SLOT(reloadPagesAndIndex(QStringList, int)));

    QObject::connect(this, SIGNAL(openBookSignal(QString)), &m_readingManager,
                     SLOT(openBook(QString)));

    QObject::connect(this, SIGNAL(closeOpenedBookAtPage(int)), &m_readingManager,
                     SLOT(closeOpenedBookAtPage(int)));

    QObject::connect(this, SIGNAL(setSectionsSignal(QVector<Section>)), &m_readingManager,
                     SLOT(setSections(QVector<Section>)));

    QObject::connect(this, SIGNAL(setNotesSignal(QMap<QString, QString>)),
                     &m_readingManager, SLOT(setNotes(QMap<QString, QString>)));

    QObject::connect(&m_readingManager, SIGNAL(finishedCalculating()), this,
                     SLOT(finishedCalculating()));
}

// threads
void Core::setUpThreadsConnections()
{
    QObject::connect(m_workingThread, SIGNAL(started()), &m_readingManager, SLOT(init()),
                     Qt::DirectConnection);

    QObject::connect(QThread::currentThread(), SIGNAL(finished()), m_workingThread,
                     SLOT(deleteLater()));
}

void Core::setUpOtherConnections()
{
    // QmlCppInterface <--> LibraryManager
    QObject::connect(&m_qmlCppInterface, SIGNAL(addNewBookByFilenameSignal(QString)),
                     &m_libraryManager, SLOT(addNewBookByFilename(QString)));

    QObject::connect(&m_qmlCppInterface, SIGNAL(deleteBookByIdSignal(int)),
                     &m_libraryManager, SLOT(deleteBookById(int)));

    QObject::connect(&m_libraryManager, SIGNAL(updateLibraryModel()), this,
                     SLOT(updateLibraryModel()));

    // LibraryManager <--> ReadingManager
    QObject::connect(&m_readingManager, SIGNAL(saveCurrentPositionForOpenedBook(int)),
                     &m_libraryManager, SLOT(saveCurrentPositionForOpenedBook(int)));
}

void Core::setUpConnections()
{
    setUpGuiWorkerConnections();
    setUpCoreWorkerConnections();
    setUpNetworkConnections();
    setUpThreadsConnections();
    setUpOtherConnections();

    //
    //    QObject::connect(&m_readingManager, SIGNAL(finished()), &m_readingThread,
    //                     SLOT(quit()));
    //    QObject::connect(&m_readingManager, SIGNAL(finished()), &m_readingManager,
    //                     SLOT(deleteLater()));
    //    QObject::connect(&m_readingThread, SIGNAL(finished()), &m_readingThread,
    //                     SLOT(deleteLater()));

    QObject::connect(QThread::currentThread(), SIGNAL(finished()), m_workingThread,
                     SLOT(deleteLater()));
    //    QObject::connect(QThread::currentThread(), SIGNAL(finished()),
    //                     QThread::currentThread(), SLOT(deleteLater()));
}

/**
 *  @brief Set up basic core settings
 *
 *  Sets up basic application settings like models for QML views and other QML properties.
 */
void Core::setUp()
{
    m_libraryManager.moveToThread(m_workingThread);
    m_readingManager.moveToThread(m_workingThread);
    m_networkManager.moveToThread(m_workingThread);

    m_workingThread->start();

    m_calculating = false;

    // set up interface between QML and C++
    m_qmlCppInterface.setCore(this);
    m_engine.rootContext()->setContextProperty("QmlCppInterface", &m_qmlCppInterface);

    // set up mouse (for correct window moving)
    m_engine.rootContext()->setContextProperty("mousePosition", &m_mousePosProvider);

    // set up words models
    m_wordsManager.setUpModels();
    m_wordsManager.loadWords();

    std::shared_ptr<MySqlQueryModel> knownModel = m_wordsManager.getKnownWordsModel();
    m_engine.rootContext()->setContextProperty("knownSqlModel", knownModel.get());

    std::shared_ptr<MySqlQueryModel> ignoredModel = m_wordsManager.getIgnoredWordsModel();
    m_engine.rootContext()->setContextProperty("ignoredSqlModel", ignoredModel.get());

    m_libraryManager.loadBooks();
    m_engine.rootContext()->setContextProperty("librarySqlModel", m_libraryModel.get());

    // set up model for book reader
    m_readingManager.setWordsManager(&m_wordsManager);

    QStringList emptyList("");
    m_bookPagesModel.setStringList(emptyList);
    m_engine.rootContext()->setContextProperty("bookModel", &m_bookPagesModel);

    m_contentsModel.setStringList(emptyList);
    m_engine.rootContext()->setContextProperty("contentsModel", &m_contentsModel);

    m_engine.rootContext()->setContextProperty("searchWordModel", &m_searchWordModel);
}

/**
 *  @brief Start the application
 *
 *  Starts the application by loading main QML form.
 */
void Core::start() { m_engine.load(QUrl(QStringLiteral("qrc:/Main.qml"))); }

/**** READING MANAGER ****/
void Core::openBook(int id)
{
    m_waitLoadingByPercents = true;

    // close old
    int index = m_qmlCppInterface.m_currentPageIndex;
    emit closeOpenedBookAtPage(index);

    clearContentModel();
    clearBookModel();

    // open book
    m_libraryManager.setOpenedBookId(id);
    QString fileName = m_libraryManager.getBookFileNameById(id);
    if (fileName.isEmpty()) return;

    m_calculating = true;
    m_qmlCppInterface.setBookOpened(true);
    emit openBookSignal(fileName);

    // set notes and section
    QMap<QString, QString> notes = m_libraryManager.getNotes(id);
    emit setNotesSignal(notes);
    m_qmlCppInterface.updateNotes(notes);

    QVector<Section> sections = m_libraryManager.getSections(id);
    emit setSectionsSignal(sections);
}

QString Core::countAllUnknownWordsByBookId(int id)
{
    QString fileName = m_libraryManager.getBookFileNameById(id);
    long result = m_readingManager.countUnknownWords();

    QString resultStr = QString::number(result);
    return resultStr;
}

void Core::clearBookModel()
{
    WCNT_LOG("[QML UPDATE] Book model cleared! Rows to remove: {}. ",
             m_bookPagesModel.rowCount());

    m_bookPagesModel.removeRows(0, m_bookPagesModel.rowCount());
}

void Core::clearContentModel()
{
    m_contentsModel.removeRows(0, m_contentsModel.rowCount());
}

void Core::wordClicked(const QString &word)
{
    m_networkManager.requestSearchResultsForWord(word);
}

void Core::addWord(const WordItem &word) { m_wordsManager.addNewWordInfo(word); }

void Core::closeProgram() { m_wordsManager.saveWords(); }

void Core::reloadPagesAndIndex(QStringList pages, int index)
{
    WCNT_LOG("Reloading pages. Pages count {}, new index {}.", pages.count(), index);

    m_bookPagesModel.setStringList(pages);

    m_qmlCppInterface.m_currentPageIndex = index;
    emit m_qmlCppInterface.pageIndexChanged();
    m_calculating = false;
}

void Core::updateLibraryModel()
{
    QSqlDatabase db = LibraryDatabase::getInstance();
    m_libraryModel->setQuery("SELECT * FROM Books", db);
}

void Core::recievedEntryResult(QVector<WordItem> words)
{
    m_searchWordModel.resetAll();
    for (auto &word : words)
    {
        m_searchWordModel.addWord(word);
    }
}

void Core::startedLoadingByPercents()
{
    clearBookModel();

    m_lostForwardPages.clear();
    m_lostBackwardPages.clear();

    m_waitLoadingByPercents = false;
}

void Core::goToPage(int index)
{
    WCNT_LOG("[QML UPDATE] Go to {} page", index);

    if (m_lostForwardPages.size() > 0)
    {
        WCNT_LOG("[QML UPDATE] Loading lost {} forward pages...",
                 m_lostForwardPages.size());
        updateForwardPages(m_lostForwardPages);
        m_lostForwardPages.clear();
    }
    if (m_lostBackwardPages.size() > 0)
    {
        WCNT_LOG("[QML UPDATE] Loading lost {} backward pages...",
                 m_lostBackwardPages.size());
        updateBackwardPages(m_lostBackwardPages);
        m_lostBackwardPages.clear();
    }

    m_qmlCppInterface.m_currentPageIndex = index;
    emit m_qmlCppInterface.pageIndexChanged();

    m_waitLoadingByPercents = false;
}

void Core::updateForwardPages(QStringList forwardPages)
{
    WCNT_LOG("Updating forward pages.");
    for (auto str : forwardPages)
    {
        QModelIndex index;
        if (m_bookPagesModel.insertRow(m_bookPagesModel.rowCount()))
        {
            index = m_bookPagesModel.index(m_bookPagesModel.rowCount() - 1, 0);
        }
        m_bookPagesModel.setData(index, str);
    }
    m_calculating = false;
}

void Core::updateBackwardPages(QStringList backwardPages)
{
    WCNT_LOG("Updating backward pages.");
    for (auto str : backwardPages)
    {
        QModelIndex index;
        if (m_bookPagesModel.insertRow(0))
        {
            index = m_bookPagesModel.index(0, 0);
        }
        m_bookPagesModel.setData(index, str);
    }
    m_calculating = false;
}

void Core::addForwardPage(QString str)
{
    if (m_waitLoadingByPercents)
    {
        m_lostForwardPages << str;
        return;
    }

    WCNT_LOG("[QML UPDATE] Adding single forward page #{} ", m_bookPagesModel.rowCount());

    QModelIndex index;
    if (m_bookPagesModel.insertRow(m_bookPagesModel.rowCount()))
    {
        index = m_bookPagesModel.index(m_bookPagesModel.rowCount() - 1, 0);
    }
    m_bookPagesModel.setData(index, str);
}

void Core::addBackwardPage(QString str)
{
    if (m_waitLoadingByPercents)
    {
        m_lostBackwardPages << str;
        return;
    }

    WCNT_LOG("[QML UPDATE] Adding single forward page");
    QModelIndex index;
    if (m_bookPagesModel.insertRow(0))

    {
        index = m_bookPagesModel.index(0, 0);
    }
    m_bookPagesModel.setData(index, str);
}

void Core::finishedCalculating() { m_calculating = false; }

void Core::updateTableOfContent(QStringList newContent)
{
    m_contentsModel.setStringList(newContent);
}
