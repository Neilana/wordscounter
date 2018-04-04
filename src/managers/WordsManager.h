#pragma once

#include <memory>
#include <set>

#include <QObject>
#include <QString>

#include "MySqlQueryModel.h"
#include "WordItem.h"
#include "WordsDatabase.h"

class WordsManager : public QObject
{
    Q_OBJECT

public:
    WordsManager(QObject* parent = 0) {}

    // models operations
    void setUpModels();
    std::shared_ptr<MySqlQueryModel> getKnownWordsModel();
    std::shared_ptr<MySqlQueryModel> getIgnoredWordsModel();

    // getters
    const std::set<QString>& getIgnoredWords() const;
    const std::map<QString, WordItem>& getKnownWordsInfo() const;

    // word operations
    void loadWords();
    void saveWords();
    void addNewWordInfo(const WordItem& wordInfo);
    bool isWordKnownOrIgnored(const QString& word);

signals:
    void changeProgramStatus(QString);

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
