#pragma once

#include <QString>
#include <QTextCursor>
#include <QTextDocument>
#include <QVector>

#include <math.h>
#include <memory>

#include <spdlog/spdlog.h>

class ReadingManager;

// forward declaration
class WordsManager;

/**
 *  @brief Class that holds book contents as an HTML-document and perfoms different
 *  opertons with it like spliting into pages, highlighting unknown words etc.
 */
class PagesGenerator : public QObject
{
    Q_OBJECT
public:
    PagesGenerator(QObject* parent = 0);
    virtual ~PagesGenerator();

    /* SETTERS */
    void setPageSize(int width, int height);
    void setNewHtmlContent(const QString& text);
    void setWordsManager(WordsManager* manager);
    void setReadingManager(ReadingManager* manager);

    /* PAGES PROCESSING (public) */
    int calculateNearbyPages(int startPos = -1);    // -1 == m_currentIndex

    // pages operations
    QStringList loadBackPages();
    QStringList loadNextPages();
    void highlightWordsOnPage(int page);

    /* GETTERS */
    QStringList getHighlightedTextFragments() const;
    QStringList getOriginalTextFragments() const;

    QString getPlainText() const;

    int foundPagesCount();
    void resetAll();

    int getCalculatedPageStartPos(int index);
    bool hasCalculatedPages();
    int getCalculatedPageWithPosition(int pos);

    std::shared_ptr<QTextDocument> m_doc;

private:
    /**
     *  @brief Small private structure for handling information about single page (that
     *  was splited from the original HTML-document).
     */
    struct SinglePage
    {
        int m_startPos;    ///< position of top left character in the original document
        int m_endPos;    ///< position of bottom right character in the original document

        QString m_originalHtml;       ///< non-highlighted text (as-is)
        QString m_highlightedHtml;    ///< highlighted text
    };

    // pages calculation
    void adjustCurrentImageHeight(QTextCursor* cursor);
    int adjustHighlightedPageHeight(int page);
    int adjustHighlightedPageHeightFromBottom(int page);

    // forward
    QString calculatePageFromTop(QTextCursor& pageCursor);
    QStringList calculateSeveralPagesForward(
        int startGlobalPos = 0, int pagesToCalculate = -1,
        bool keepOldPages = false);    // -1 = all (til the end)

    // backward
    QString calculatePageFromBottom(QTextCursor& pageCursor, bool& wasFirstLine);
    QStringList calculateSeveralPagesBackward(
        int startGlobalPos = 10005, int pagesToCalculate = -1,
        bool keepOldPages = false);    // -1 = all (til the begining)

    int m_docWidth;     ///< width of the book page
    int m_docHeight;    ///< height of the book page

    WordsManager* m_wordsManager;        ///< pointer to the dictinory
    ReadingManager* m_readingManager;    ///< pointer to the dictinory

    QVector<SinglePage> m_calculatedPages;    ///< pages that program managed to calculate

    QString m_htmlBookText;
    bool m_highlightWords;    ///< should the unknown words be highlighted
};
