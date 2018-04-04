// .h
#include "Config.h"

// qt
#include "MainWindow.h"
///#include "ui_mainwindow.h"

#include <QDebug>
#include <QTextStream>

// полезные штуки
#include <fstream>
#include <map>
#include <string>

// моё
#include "LibraryBook.h"
#include "LibraryDatabase.h"
#include "WordsDatabase.h"

#include "functions/textFuncs.h"       // для get/put WordsFromStream
#include "functions/windowsFuncs.h"    // для showHighlightedStream

using namespace std;

Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

// Config::Config() : m_knownFName(KNOWN_FNAME), m_ignoredFName(IGNORED_FNAME)
// {}

Config::Config()
{
    m_colors[WordType::Known] = "black";
    m_colors[WordType::Unknown] = "blue";
    m_colors[WordType::Ignored] = "magenta";
}

/*
void Config::loadWords()
{
    WordsDatabase db = WordsDatabase::getInstance();
    // db.open();
    QSqlQuery query;
    query.exec("SELECT * FROM KnownWords");
    while (query.next())
    {
        std::string word = query.value(1).toString().toStdString();
        std::string definition = query.value(2).toString().toStdString();

        WordClass bufWord(word, definition);
        m_knownWordsMap[word] = bufWord;
    }

    query.exec("SELECT * FROM IgnoredWords");
    while (query.next())
    {
        std::string word = query.value(1).toString().toStdString();
        m_ignoredWords.insert(word);
    }
}

void Config::saveWords() const
{
    WordsDatabase db = WordsDatabase::getInstance();
    // db.open();

    for (auto it : m_newWordsMap)
    {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO KnownWords (Word, Definition) "
            "VALUES (:word, :definition)");
        query.bindValue(":word", QString::fromStdString(it.first));
        query.bindValue(":definition", QString::fromStdString(it.second.getDefinition()));
    }

    for (auto it : m_newIgnoredWords)
    {
        QSqlQuery query;
        query.prepare(
            "INSERT INTO IgnoredWords (Word) "
            "VALUES (:word)");
        query.bindValue(":word", QString::fromStdString(it));
        if (!query.exec())
        {
            qDebug() << query.lastError().text();
            qDebug() << query.lastQuery();
        }
    }
}*/

void Config::setTextFileName(const std::string &fileName)
{
    // проверки ?
    m_textFName = fileName;
    // qDebug() << m_textFName;
}
/*
void Config::addKnownWord(const WordClass &wordInfo)
{
    const string word = wordInfo.getWord();
    m_knownWordsMap[word] = wordInfo;
}

void Config::addNewWordInfo(const WordClass &wordInfo)
{
    const string word = wordInfo.getWord();
    m_newWordsMap[word] = wordInfo;
}*/

/*
void Config::loadBooks()
{
    LibraryDatabase db = LibraryDatabase::getInstance();
    // db.open();
    QSqlQuery query;
    query.exec("SELECT * FROM Books");
    while (query.next())
    {
        int id = query.value(0).toInt();
        std::string author = query.value(1).toString().toStdString();
        std::string title = query.value(2).toString().toStdString();
        std::string fileName = query.value(4).toString().toStdString();

        LibraryBook bufBook(id, author, title, fileName);
        // qDebug() << "HERE " << id;
        m_books[id] = bufBook;
    }
}*/

void Config::setCurrentBook(int id)
{
    // m_textFName =
    currentBookId = id;
    // HAHAHA m_textFName = m_books[id].getFileName();
}
