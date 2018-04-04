#include "LibraryManager.h"

#include <QCryptographicHash>
#include <QDomDocument>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QUrl>
#include <QXmlQuery>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QXmlStreamReader>

#include "LibraryDatabase.h"
#include "MySqlQueryModel.h"
#include "loggerDefine.h"

LibraryManager::LibraryManager(QObject *parent) : QObject(parent)
{
    m_userId = 1;    // FIXME
}

QString LibraryManager::calculateBookMd5(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        if (hash.addData(&file))
        {
            std::string hashStr = hash.result().toHex().toStdString();
            return QString::fromStdString(hashStr);
        }
    }
    return QString();
}

bool LibraryManager::isHashInDatabase(const QString &hashSum)
{
    for (auto &book : m_books)
        if (book.second.getMd5() == hashSum)
        {
            return true;
        }
    return false;
}

const QMap<QString, QString> &LibraryManager::getNotes(int id) const
{
    if (m_books.count(id) > 0)
        return m_books.at(id).m_notes;
    else
        return {};    // local referenece (should not reach this point)
}

const QVector<Section> &LibraryManager::getSections(int id) const
{
    if (m_books.count(id) > 0)
        return m_books.at(id).m_sections;
    else
        return {};    // local referenece (should not reach this point)
}

void LibraryManager::loadBooks()
{
    QSqlDatabase db = LibraryDatabase::getInstance();

    WCNT_LOG("[DB] Loading books from database...");

    QSqlQuery query(db);
    query.exec("SELECT * FROM Books");
    if (!query.isActive())
    {
        WCNT_LOG("[DB] Error: {}\nLast query: {}", query.lastError().text().toStdString(),
                 query.lastQuery().toStdString());
    }

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString author = query.value(1).toString();
        QString title = query.value(2).toString();
        QString fileName = query.value(4).toString();
        QString md5 = query.value(5).toString();
        QString lang = query.value(6).toString();
        // int highlightWordsInt = query.value(6).toInt();
        // bool highlightWordsBool = (highlightWordsInt == 0) ? false : true;

        // SECTIONS
        QSqlQuery query2(db);
        query2.prepare(
            "SELECT * FROM BooksSections WHERE BookId = :bookId ORDER BY SectionId ASC");
        query2.bindValue(":bookId", id);

        QVector<Section> sections;
        query2.exec();
        while (query2.next())
        {
            // int sectionId = query.value(1).toInt();
            int sectionLevel = query.value(2).toInt();
            QString name = query2.value(3).toString();
            int position = query2.value(4).toInt();

            Section bufSection;
            bufSection.m_position = position;
            bufSection.m_title = name;
            bufSection.m_level = sectionLevel;

            sections.push_back(bufSection);
        };

        // NOTES
        QSqlQuery query3(db);
        query3.prepare("SELECT * FROM BooksNotes WHERE BookId = :bookId");
        query3.bindValue(":bookId", id);

        QMap<QString, QString> notes;
        query3.exec();
        while (query3.next())
        {
            QString noteId = query3.value(1).toString();
            QString name = query3.value(2).toString();

            notes[noteId] = name;
        };

        LibraryBook bufBook(id, author, title, fileName, md5, lang);
        bufBook.m_sections = sections;
        bufBook.m_notes = notes;
        m_books[id] = bufBook;
    }
}

const QString &LibraryManager::getBookMd5ById(int id) const
{
    if (m_books.count(id) > 0)
        return m_books.at(id).getMd5();
    else
        return {};    // WARNING: local referenece (should not reach this point)
}

const QString &LibraryManager::getBookFileNameById(int id) const
{
    if (m_books.count(id) > 0)
        return m_books.at(id).getFileName();
    else
        return {};    // WARNING: local referenece (should not reach this point)
}

void LibraryManager::setOpenedBookId(int id) { m_openedBookId = id; }

int LibraryManager::insertBookIntoDb(const QString &title, const QString &author,
                                     const QString &image, const QString &path,
                                     const QString &hashSum, const QString &language)
{
    int insertedBookId = -1;
    QSqlDatabase db = LibraryDatabase::getInstance();

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO Books (Title, Author, Image, Path, HashSum, Language) "
        "VALUES (:title, :author, :image, :path, :hashSum, :language)");
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":image", image);
    query.bindValue(":path", path);
    query.bindValue(":hashSum", hashSum);
    query.bindValue(":language", language);
    if (!query.exec())
    {
        WCNT_LOG("Error: {}", query.lastError().text().toStdString());
        WCNT_LOG("Last query: {} ", query.lastQuery().toStdString());

        insertedBookId = -1;
    }
    else
        insertedBookId = query.lastInsertId().toInt();
    return insertedBookId;
}

void LibraryManager::insertSectionsIntoDb(int bookId, QVector<Section> &sections)
{
    QSqlDatabase db = LibraryDatabase::getInstance();

    int sectionId = 0;
    for (auto section : sections)
    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO BooksSections (BookId, SectionId, SectionLevel, Name, Position) "
            "VALUES (:bookId, :sectionId, :sectionLevel, :name, :position)");
        query.bindValue(":bookId", bookId);
        query.bindValue(":sectionId", sectionId++);
        query.bindValue(":sectionLevel", section.m_level);
        query.bindValue(":name", section.m_title);
        query.bindValue(":position", section.m_position);
        if (!query.exec())
        {
            WCNT_LOG("Error: {}", query.lastError().text().toStdString());
            WCNT_LOG("Last query: {} ", query.lastQuery().toStdString());
        }
    }
}

void LibraryManager::insertNotesIntoDb(int bookId, QMap<QString, QString> &notes)
{
    QSqlDatabase db = LibraryDatabase::getInstance();
    for (auto note :
         notes.toStdMap())    // NOTE: otherwiswe qt not iterate (first/second)
    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO BooksNotes (BookId, NoteId, Name) "
            "VALUES (:bookId, :noteId, :name)");
        query.bindValue(":bookId", bookId);
        query.bindValue(":noteId", note.first);
        query.bindValue(":name", note.second);
        if (!query.exec())
        {
            WCNT_LOG("Error: {}", query.lastError().text().toStdString());
            WCNT_LOG("Last query: {} ", query.lastQuery().toStdString());
        }
    }
}

LibraryManager::TitleInfoFb2 LibraryManager::readTitleInfoFromFb2(const QString &fileName)
{
    QDomDocument doc;
    QFile file(fileName);

    QString errorMessage;
    int errorLine, errorColumn;

    // FIXME: strange error handling
    if (!file.open(QIODevice::ReadOnly) ||
        !doc.setContent(&file, &errorMessage, &errorLine, &errorColumn))
    {
        qDebug() << "XML Error at line" << errorLine << "column" << errorColumn << ":"
                 << errorMessage;
        return TitleInfoFb2();
    }
    QDomNode titleInfo = doc.elementsByTagName("title-info").at(0);
    // book title
    // FIXME: error handling (no such node)
    QString bookTitle =
        titleInfo.firstChildElement("book-title").firstChild().nodeValue();
    WCNT_LOG("Title: {}", bookTitle.toStdString());

    // authors
    QString authorsStr;
    QDomNodeList authors = titleInfo.toElement().elementsByTagName("author");
    for (int i = 0; i < authors.size(); i++)
    {
        QDomNode authorNode = authors.item(i);
        QDomElement firstName = authorNode.firstChildElement("first-name");
        QDomElement lastName = authorNode.firstChildElement("last-name");
        if (authorsStr != "") authorsStr += ", ";
        authorsStr += firstName.text() + " " + lastName.text();
        WCNT_LOG("Author: {} {}", firstName.text().toStdString(),
                 lastName.text().toStdString());
    }

    // image
    QDomNode image = titleInfo.firstChildElement("coverpage").firstChildElement("image");
    if (image.isNull()) image = doc.elementsByTagName("image").at(0);

    QString imageId = image.attributes().item(0).nodeValue();
    imageId.remove(0, 1);    // remove '#' symbol

    QString imageBinary;
    QDomNodeList binaries = doc.elementsByTagName("binary");

    for (int i = 0; i < binaries.size(); i++)
    {
        QDomNode binaryNode = binaries.item(i);
        if (binaryNode.attributes().namedItem("id").nodeValue() == imageId)
        {
            imageBinary = binaryNode.firstChild().nodeValue();
        }
    }
    // language
    QString language = titleInfo.firstChildElement("lang").text();
    TitleInfoFb2 bufTitleInfo{bookTitle, authorsStr, imageBinary, language};
    return bufTitleInfo;
}

/* "Books" (`Title` TEXT, `Author` TEXT, `Image` BLOB, `Path` TEXT, `HashSum` TEXT,
 * `HighlightWords` INTEGER DEFAULT 1 )*/
void LibraryManager::addNewBookByFilename(const QString &fileName)
{
    QString hashSum = calculateBookMd5(fileName);

    bool bookWasAdded = isHashInDatabase(hashSum);
    if (bookWasAdded)
    {
        emit changeProgramStatus("Book already exists in db!");
        return;
    }

    emit changeProgramStatus("Adding book in db...");
    // validateFb2Format(fileName);
    TitleInfoFb2 bufTitle = readTitleInfoFromFb2(fileName);

    WCNT_LOG("Will try to add new book: {}", fileName.toStdString());

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString htmlBookText;
    QXmlQuery xmlQuery(QXmlQuery::XSLT20);
    // CHECK FOR fb2???
    xmlQuery.setFocus(&file);
    xmlQuery.setQuery(QUrl("qrc:/myStylesheet_sections.xsl"));
    if (xmlQuery.evaluateTo(&htmlBookText))
    {
        WCNT_LOG("File is valid!")
    }
    else
    {
        WCNT_LOG("File is invalid :(");
    }

    QString title = bufTitle.m_title;
    QString author = bufTitle.m_author;
    QString image = bufTitle.m_image;
    QString path = fileName;

    QString language = bufTitle.m_language;
    // bool highlightWords = 1;

    // add to db
    int id = insertBookIntoDb(title, author, image, path, hashSum, language);

    // generate contents
    m_sections.clear();
    m_notes.clear();

    emit changeProgramStatus("Generating table of contents...");
    readSectionsTitlesAndNotesFromFb2(fileName);
    readSectionsPositions(htmlBookText);

    // FIXME: not true way to connect to DB
    QSqlDatabase::database("LibraryDB").transaction();
    insertSectionsIntoDb(id, m_sections);
    insertNotesIntoDb(id, m_notes);
    QSqlDatabase::database("LibraryDB").commit();

    /* `BooksContents` ( `BookId` INTEGER, `ContentId` INTEGER, `ContentLevel` INTEGER NOT
     * NULL DEFAULT 0, `Name` TEXT NOT NULL, `Position` INTEGER NOT NULL DEFAULT 0,
     * PRIMARY KEY(`BookId`,`ContentId`) ) */

    // add to internal map
    if (id > -1)
    {
        LibraryBook bufBook(id, author, title, path, hashSum, language);
        bufBook.m_sections = m_sections;
        bufBook.m_notes = m_notes;

        m_books[id] = bufBook;

        // update model
        emit updateLibraryModel();
        emit changeProgramStatus("Book added successfully!");
    }
}

void LibraryManager::deleteBookById(int id)
{
    QSqlDatabase db = LibraryDatabase::getInstance();
    WCNT_LOG("Delete the book with id: {}", id);

    emit changeProgramStatus("Deleting the book...");

    QSqlQuery query1(db);
    QSqlQuery query3(db);
    QSqlQuery query4(db);
    QSqlQuery query5(db);

    query1.prepare("DELETE FROM Books WHERE id = :id");

    QSqlQuery query2(db);
    query2.prepare("DELETE FROM UsersProgress WHERE BookId = :id AND UserId = :userId");
    query2.bindValue(":id", id);
    query2.bindValue(":userId", m_userId);

    query3.prepare("DELETE FROM Bookmarks WHERE BookId = :id");
    query4.prepare("DELETE FROM BooksSections WHERE BookId = :id");
    query5.prepare("DELETE FROM BooksNotes WHERE BookId = :id");

    query1.bindValue(":id", id);
    query3.bindValue(":id", id);
    query4.bindValue(":id", id);
    query5.bindValue(":id", id);

    if (!query1.exec())
    {
        WCNT_LOG("[DB] Error: {}\n Last query {}",
                 query1.lastError().text().toStdString(),
                 query1.lastQuery().toStdString());
    }

    if (!query2.exec())
    {
        WCNT_LOG("[DB] Error: {}\n Last query {}",
                 query2.lastError().text().toStdString(),
                 query2.lastQuery().toStdString());
    }

    if (!query3.exec())
    {
        WCNT_LOG("[DB] Error: {}\n Last query {}",
                 query3.lastError().text().toStdString(),
                 query3.lastQuery().toStdString());
    }

    if (!query4.exec())
    {
        WCNT_LOG("[DB] Error: {}\n Last query {}",
                 query4.lastError().text().toStdString(),
                 query4.lastQuery().toStdString());
    }

    if (!query5.exec())
    {
        WCNT_LOG("[DB] Error: {}\n Last query {}",
                 query5.lastError().text().toStdString(),
                 query5.lastQuery().toStdString());
    }

    m_books.erase(id);

    // update model
    emit updateLibraryModel();
    emit changeProgramStatus("Book was deleted!");
}

void LibraryManager::saveCurrentPositionForOpenedBook(int pos)
{
    QSqlQuery query;
    query.prepare(
        "REPLACE INTO UsersProgress(UserId, BookId, LastPosition) VALUES (:userId, "
        ":bookId, :position)");
    query.bindValue(":position", pos);
    query.bindValue(":userId", m_userId);
    query.bindValue(":bookId", m_openedBookId);

    if (!query.exec())
    {
        WCNT_LOG("Error: {}", query.lastError().text().toStdString());
        WCNT_LOG("Last query: {} ", query.lastQuery().toStdString());
    }
}

bool LibraryManager::isLink(const QString &htmlStr)
{
    QDomDocument doc;
    doc.setContent(htmlStr);
    // FIXME: body notes ??? or all links
    QDomNode linkNode = doc.elementsByTagName("a").at(0);
    bool wasLink = false;
    if (!linkNode.isNull())
    {
        wasLink = true;
    }
    return wasLink;
}

QString LibraryManager::extractSectionLinkName(const QString &htmlStr)
{
    QString linkName;
    QDomDocument doc;
    doc.setContent(htmlStr);
    // FIXME: body notes ??? or all links
    QDomNodeList linksNodes = doc.elementsByTagName("a");

    for (int i = 0; i < linksNodes.size(); i++)
    {
        QDomNode node = linksNodes.item(i);
        linkName = node.attributes().namedItem("name").nodeValue();
        if (linkName.startsWith("SECTION_"))
            return linkName;
        else
            linkName = "";
    }

    return linkName;
}

// initial fb2
void LibraryManager::readSectionsTitlesAndNotesFromFb2(const QString &fileName)
{
    QDomDocument doc;
    QFile file(fileName);

    QString errorMessage;
    int errorLine, errorColumn;

    // FIXME: strange error handling
    if (!file.open(QIODevice::ReadOnly) ||
        !doc.setContent(&file, &errorMessage, &errorLine, &errorColumn))
    {
        qDebug() << "XML Error at line" << errorLine << "column" << errorColumn << ":"
                 << errorMessage;
        return;
    }

    // SECTIONS
    QDomNodeList bodies = doc.elementsByTagName("body");
    for (int i = 0; i < bodies.size(); i++)
    {
        QDomNode body = bodies.item(i);
        if (body.attributes().namedItem("name").nodeValue() != "notes" &&
            body.attributes().namedItem("name").nodeValue() != "footnotes")
        {
            QDomNode section = body.firstChildElement("section");
            scanForSectionsTitles(section, 0);    // recursion
        }
        else
        //         if (body.attributes().namedItem("name").nodeValue() == "notes")
        {
            QDomNode note = body.firstChildElement("section");
            while (!note.isNull())
            {
                QString noteId = note.attributes().namedItem("id").nodeValue();
                QString noteValue = note.toElement().text();
                m_notes[noteId] = noteValue;
                note = note.nextSiblingElement("section");
            }
        }
    }

    //    // notes that are not typical
    //    QDomNodeList as = doc.elementsByTagName("a");
    //    qDebug() << "Size: " << as.size();
    //    // get all notes names
    //    for (int i = 0; i < as.size(); i++)
    //    {
    //        QDomNode aNode = as.item(i);
    //        QString aType = aNode.attributes().namedItem("type").nodeValue();
    //        if (aType == "note")
    //        // if (aType != "")
    //        {
    //            qDebug() << "Type : " << aType;
    //            // QString id = aNode.attributes().namedItem("href").nodeValue();
    //            QString id = aNode.attributes().namedItem("id").nodeValue();
    //            qDebug() << "Found note: " << id << i;
    //            if (id != "" && id[0] == '#')
    //            {
    //                id.remove(0, 1);    // remove '#'
    //                // QString noteValue = doc.elementById(id).toElement().text();
    //                // m_notes[id] = noteValue;
    //                m_notes[id] = "";
    //            }
    //        }
    //    }

    //    // notes that are not typical
    //    QDomNodeList as = doc.elementsByTagName("a");
    //    qDebug() << "Size: " << as.size();
    //    //    // get all notes names
    //    //    for (int i = 0; i < as.size(); i++)
    //    //    {
    //    //        QDomNode aNode = as.item(i);
    //    //        QString aType = aNode.attributes().namedItem("type").nodeValue();
    //    //        // if (aType == "note")
    //    //        // if (aType != "")
    //    //        {
    //    //            QString id = aNode.attributes().namedItem("name").nodeValue();
    //    //            // if (id[0] == '#')
    //    //            {
    //    //                id.remove(0, 1);    // remove '#'
    //    //                // QString noteValue = doc.elementById(id).toElement().text();
    //    //                // m_notes[id] = noteValue;
    //    //                m_notes[id] = "";
    //    //                qDebug() << id;
    //    //            }
    //    //        }
    //    //    }

    //    // get notes values
    //    for (int i = 0; i < as.size(); i++)
    //    {
    //        QDomNode aNode = as.item(i);
    //        // QString id = aNode.attributes().namedItem("type").nodeValue();
    //        QString id = aNode.attributes().namedItem("name").nodeValue();
    //        // if (aType == "note")
    //        // if (id[0] == '#')
    //        qDebug() << id;
    //        {
    //            //      id.remove(0, 1);    // remove '#'
    //            if (id != "" && m_notes.count(id) > 0)
    //            {
    //                // QString id = aNode.attributes().namedItem("href").nodeValue();
    //                QString noteValue = aNode.toElement().text();
    //                m_notes[id] = noteValue;
    //            }
    //        }
    //    }
}

void LibraryManager::scanForSectionsTitles(const QDomNode &section, int level)
{
    QString title;

    QDomElement sectionTitle = section.firstChildElement("title");
    if (sectionTitle.isNull())
    {
        QDomNode sectionNode = section.firstChild();
        if (sectionNode.nodeName() != "section")
        {
            sectionTitle = sectionNode.toElement();
            title = sectionNode.toElement().text();
            while (title == "" && sectionNode.nodeName() != "section" &&
                   !sectionNode.isNull())
            {
                sectionNode = sectionNode.nextSibling();
                title = sectionNode.toElement().text();
                sectionTitle = sectionNode.toElement();
            }
        }
    }

    if (!sectionTitle.isNull())
    {
        Section bufSection;
        title = "";
        for (QDomNode n = sectionTitle.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            title += n.toElement().text() + " ";
        }

        title = title.trimmed();
        if (title.length() > 140)
        {
            title.resize(140);
            title += "...";
        }

        bufSection.m_title = title;
        bufSection.m_level = level;
        bufSection.m_position = 0;

        m_sections.push_back(bufSection);
    }

    if (!section.firstChildElement("section").isNull())
    {
        scanForSectionsTitles(section.firstChildElement("section"), level + 1);
    }

    if (!section.nextSiblingElement("section").isNull())
    {
        scanForSectionsTitles(section.nextSiblingElement("section"), level);
    }
}

// fb2 + xsl as html
void LibraryManager::readSectionsPositions(const QString &htmlText)
{
    QTextDocument htmlDoc;
    htmlDoc.setDefaultStyleSheet(
        "body {text-align:justify; font-family: \"Heuristica\" ; font-size: 19px; "
        "color: #433B32; } "
        "p { font-family: \"Heuristica\" ; font-size: 19px; color: #433B32; } "
        "a { color: #433B32; }");
    htmlDoc.setHtml(htmlText);

    QTextOption opt;
    opt.setWrapMode(QTextOption::WordWrap);

    htmlDoc.setDocumentMargin(0);
    htmlDoc.setDefaultTextOption(opt);

    QDomDocument doc;
    doc.setContent(htmlDoc.toHtml());

    int sectionN = 0;
    QTextCursor cursor(&htmlDoc);

    while (!cursor.atEnd())
    {
        cursor.select(QTextCursor::BlockUnderCursor);
        QString htmlBlock = cursor.selection().toHtml();
        QString linkName;
        if (isLink(htmlBlock))
        {
            linkName = extractSectionLinkName(htmlBlock);
            if (!linkName.isEmpty() && linkName.startsWith("SECTION_"))
            {
                if (sectionN == 0 ||
                    m_sections[sectionN - 1].m_position != cursor.anchor())
                    m_sections[sectionN++].m_position = cursor.anchor() + 1;
            }
        }
        cursor.movePosition(QTextCursor::NextBlock);
    }
}
