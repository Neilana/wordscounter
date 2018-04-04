#pragma once

#include <memory>
#include <set>

#include <QString>

#include "MySqlQueryModel.h"
#include "WordsDatabase.h"

#include "WordItem.h"

class WordsManager
{
public:
    // models operations
    void setUpModels();
    std::shared_ptr<MySqlQueryModel> getKnownWordsModel();
    std::shared_ptr<MySqlQueryModel> getIgnoredWordsModel();

    // getters
    const std::set<QString>& getIgnoredWords() const;
    const std::map<QString, WordItem>& getKnownWordsInfo() const;

    // word operations
    void loadWords();
    void saveWords() const;
    void addNewWordInfo(const WordItem& wordInfo);
    bool isWordKnownOrIgnored(const QString& word);

private:
    std::shared_ptr<MySqlQueryModel> m_knownModel;
    std::shared_ptr<MySqlQueryModel> m_ignoredModel;

    std::map<QString, WordItem> m_knownWordsMap;
    std::map<QString, WordItem> m_newWordsMap;

    std::set<QString> m_ignoredWords;
    std::set<QString> m_newIgnoredWords;
};

inline const std::set<QString>& WordsManager::getIgnoredWords() const
{
    return m_ignoredWords;
}
inline const std::map<QString, WordItem>& WordsManager::getKnownWordsInfo() const
{
    return m_knownWordsMap;
}
