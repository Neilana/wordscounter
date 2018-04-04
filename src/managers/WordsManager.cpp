#include "WordsManager.h"

#include <QSqlError>
#include <QSqlQuery>

#include "loggerDefine.h"

void WordsManager::setUpModels()
{
    QSqlDatabase db = WordsDatabase::getInstance();

    m_knownModel = std::make_shared<MySqlQueryModel>();
    m_knownModel->setQuery("SELECT  * FROM KnownWords", db);

    m_ignoredModel = std::make_shared<MySqlQueryModel>();
    m_ignoredModel->setQuery("SELECT * FROM IgnoredWords", db);
}

std::shared_ptr<MySqlQueryModel> WordsManager::getKnownWordsModel()
{
    return m_knownModel;
}

std::shared_ptr<MySqlQueryModel> WordsManager::getIgnoredWordsModel()
{
    return m_ignoredModel;
}

void WordsManager::loadWords()
{
    QSqlDatabase db = WordsDatabase::getInstance();

    QSqlQuery query(db);
    query.exec("SELECT * FROM KnownWords");

    while (query.next())
    {
        QString word = query.value(1).toString();
        QString definition = query.value(2).toString();

        WordItem bufWord(word, definition);
        m_knownWordsMap[word] = bufWord;
    }

    query.exec("SELECT * FROM IgnoredWords");
    while (query.next())
    {
        QString word = query.value(1).toString();
        m_ignoredWords.insert(word);
    }
}

void WordsManager::saveWords() const
{
    QSqlDatabase db = WordsDatabase::getInstance();

    for (auto it : m_newWordsMap)
    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO KnownWords (Word, Definition) "
            "VALUES (:word, :definition)");
        query.bindValue(":word", it.first);
        query.bindValue(":definition", it.second.getDefinition());

        if (!query.exec())
        {
            WCNT_LOG("[DB] Error (WordsManager): {} \n Last query: ",
                     query.lastError().text().toStdString(),
                     query.lastQuery().toStdString());
        }
    }

    for (auto it : m_newIgnoredWords)
    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO IgnoredWords (Word) "
            "VALUES (:word)");
        query.bindValue(":word", it);
        if (!query.exec())
        {
            WCNT_LOG("[DB] Error (WordsManager): {} \n Last query: ",
                     query.lastError().text().toStdString(),
                     query.lastQuery().toStdString());
        }
    }
}

void WordsManager::addNewWordInfo(const WordItem &wordInfo)
{
    const QString word = wordInfo.getWord();
    m_newWordsMap[word] = wordInfo;

    saveWords();
    // update
    QSqlDatabase db = WordsDatabase::getInstance();
    m_knownModel->setQuery("SELECT  * FROM KnownWords", db);
}

bool WordsManager::isWordKnownOrIgnored(const QString &word)
{
    if ((m_knownWordsMap.count(word) == 0) && (m_newWordsMap.count(word) == 0) &&
        (m_ignoredWords.count(word) == 0) && (m_newIgnoredWords.count(word) == 0))
        return false;
    else
        return true;
}
