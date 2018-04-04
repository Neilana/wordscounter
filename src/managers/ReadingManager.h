/// @file ReadingManager.h

#pragma once

#include <QDomNode>
#include <QStringListModel>
#include <QThread>
#include <QTimer>

#include <spdlog/spdlog.h>

#include "LibraryBook.h"
#include "PagesGenerator.h"
#include "WordsManager.h"

/**
 *  @brief Class that controlls access to the book content.
 */
class ReadingManager : public QObject
{
    Q_OBJECT
public:
    ReadingManager(QObject* parent = 0);
    ~ReadingManager();

    // book content settings
    void setWordsHighlighting(bool value);

    // get some info about the book
    int getCurrentIndex();
    long countUnknownWords();

    // dictionary
    void setWordsManager(WordsManager* manager);

signals:
    void updateTableOfContent(QStringList);
    void finishedCalculating();
    void updateNotesSignal(QMap<QString, QString>);

    void updateBackwardPages(QStringList);
    void updateForwardPages(QStringList);
    void reloadPagesAndIndex(QStringList, int);

    // FIXME: MOVE TO cpp ot interf
    void clearBookModel();
    void clearContentModel();
    void addForwardPage(QString);
    void addBackwardPage(QString);

    void saveCurrentPositionForOpenedBook(int);
    void changeProgramStatus(QString);

    void startedLoadingByPercents();
    void goToPage(int);

public slots:
    void onTimeout();
    void openBook(QString fileName);
    void setPageSize(int width, int height, int index);

    void loadNextPages();
    void loadBackPages();

    void loadPageByPercents(QString percentsStr);
    void loadFromContentsTable(int);

    void init();
    void closeOpenedBookAtPage(int);

    void setNotes(QMap<QString, QString> notes);
    void setSections(QVector<Section> sections);

private:
    QSet<QString> readLinksIdsFromFb2();
    void readLinksPositions(QSet<QString>);
    QStringList getContentStringList();
    int positionToPercents(int pos);

    // FIXME: may be unique_ptr?
    PagesGenerator* m_pagesGenerator;    ///< contains HTML-formatted book content
    WordsManager* m_wordsManager;        ///< access to dictionary

    bool m_highlightUnknownWords;    ///< should the words in the book be highlighted
    int m_currentIndex;
    bool m_loadingContentOrSize;
    QString m_fileName;

    QVector<Section> m_sections;    // FIXME: pointers?
    QMap<QString, QString> m_notes;

    QString m_tableOfContents;
    bool m_timeoutOn;

    QTimer* m_timer;    // unique
};
