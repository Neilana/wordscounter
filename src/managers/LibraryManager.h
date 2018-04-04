/// @file LibraryManager.h

#pragma once

#include <QDomNode>
#include <QSqlError>
#include <QSqlQuery>

#include <spdlog/spdlog.h>
#include <memory>

#include "LibraryBook.h"
#include "LibraryDatabase.h"
#include "MySqlQueryModel.h"
#include "loggerDefine.h"

/**
 *  @brief Class for managing operatins with library database.
 */
class LibraryManager : public QObject
{
    Q_OBJECT
public:
    LibraryManager(QObject *parent = 0);

    void loadBooks();
    void setOpenedBookId(int id);

    const QString &getBookMd5ById(int id) const;
    const QString &getBookFileNameById(int id) const;
    const QMap<QString, QString> &getNotes(int id) const;
    const QVector<Section> &getSections(int id) const;

signals:
    void updateLibraryModel();
    void changeProgramStatus(QString);

public slots:
    void addNewBookByFilename(const QString &fileName);
    void deleteBookById(int id);
    void saveCurrentPositionForOpenedBook(int);

private:
    struct TitleInfoFb2
    {
        QString m_title;
        QString m_author;
        QString m_image;
        QString m_language;
    };

    // reading sections and notes from book to add
    TitleInfoFb2 readTitleInfoFromFb2(const QString &fileName);
    void readSectionsTitlesAndNotesFromFb2(const QString &);
    void readSectionsPositions(const QString &);
    void scanForSectionsTitles(const QDomNode &section, int level);

    bool isLink(const QString &htmlStr);
    QString extractSectionLinkName(const QString &htmlStr);

    // for adding new book
    QString calculateBookMd5(const QString &fileName);
    bool isHashInDatabase(const QString &fileName);

    // insertion into db
    void insertSectionsIntoDb(int id, QVector<Section> &sections);
    void insertNotesIntoDb(int bookId, QMap<QString, QString> &notes);
    int insertBookIntoDb(const QString &title, const QString &author,
                         const QString &image, const QString &path,
                         const QString &hashSum, const QString &language);

    std::map<int, LibraryBook> m_books;    ///< all books form database (by id)

    int m_userId;          ///< current user id
    int m_openedBookId;    ///< id of book being read

    QVector<Section> m_sections;       ///< table of contents of opened book
    QMap<QString, QString> m_notes;    ///< footnotes contained in the opened book
};
