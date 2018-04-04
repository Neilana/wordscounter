/// @file Core.h

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QStringListModel>
#include <QThread>

#include <spdlog/spdlog.h>

// for qml connection
#include "CursorPosProvider.h"
#include "MySqlQueryModel.h"
#include "QmlCppInterface.h"
#include "SearchWordModel.h"

#include "LibraryManager.h"
#include "NetworkManager.h"
#include "ReadingManager.h"
#include "WordsManager.h"

/**
 *  @brief Core class.
 *
 *  Main class for the application. It starts the application, handles threads and all
 * communication between GUI and other components of the program.
 */

class Core : public QObject
{
    Q_OBJECT
public:
    Core(QObject* parent = 0);
    ~Core();

    // basic
    void setUp();
    void start();

    // reading manager
    void openBook(int id);
    void loadNextPages(int page);
    void highlightWordsOnPage(int page);
    QString countAllUnknownWordsByBookId(int id);
    void clearBookModel();
    void clearContentModel();
    void wordClicked(const QString& word);
    void addWord(const WordItem& word);
    void closeProgram();

    bool m_waitLoadingByPercents;
    bool m_calculating;

signals:
    void openBookSignal(QString fileName);
    void closeOpenedBookAtPage(int);
    void setNotesSignal(QMap<QString, QString>);
    void setSectionsSignal(QVector<Section>);

public slots:
    void updateTableOfContent(QStringList);
    void updateLibraryModel();
    void updateForwardPages(QStringList);
    void updateBackwardPages(QStringList);

    void addForwardPage(QString);
    void addBackwardPage(QString);

    void finishedCalculating();
    void reloadPagesAndIndex(QStringList, int);
    void startedLoadingByPercents();
    void goToPage(int);

    // network
    void recievedEntryResult(QVector<WordItem>);

private:
    // basic set up
    void setUpConnections();
    void setUpNetworkConnections();
    void setUpGuiWorkerConnections();
    void setUpCoreWorkerConnections();
    void setUpThreadsConnections();
    void setUpOtherConnections();

    void setUpModels();
    void registerMetaTypes();

    // something related to QML
    QQmlApplicationEngine m_engine;          ///< engine for starting a QML apllication
    QmlCppInterface m_qmlCppInterface;       ///< interface between QML and C++
    CursorPosProvider m_mousePosProvider;    ///< provides correct mouse global position

    // models
    QStringListModel m_bookPagesModel;    ///< list of HTML-pages for QML-model
    QStringListModel m_contentsModel;     ///< table of contents
    SearchWordModel m_searchWordModel;    ///< search results for clicked word
    std::shared_ptr<MySqlQueryModel> m_libraryModel;    ///< displays books from database

    // managers
    ReadingManager m_readingManager;    ///< manages opertiosn with book being read
    WordsManager m_wordsManager;        ///< manages operations with dictionary
    LibraryManager m_libraryManager;    ///< manages operations with library
    NetworkManager m_networkManager;    ///< sends/recieves requests to API

    // threads
    QThread* m_workingThread;    ///< thread for calculations

    // lost pages
    QStringList m_lostForwardPages;     ///<  forward pages that were calcultaed but lost
    QStringList m_lostBackwardPages;    ///< backward pages that were calcultaed but lost
};
