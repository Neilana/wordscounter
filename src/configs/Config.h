#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

//#include "LibraryBook.h"
//#include "WordClass.h"

namespace
{
enum class WordType
{
    Known,
    Unknown,
    Ignored
};
// const std::string KNOWN_FNAME = "../wordscounter_saves/knownWords.txt";
// const std::string IGNORED_FNAME = "../wordscounter_saves/ignoredWords.txt"
}    // namespace

class Config
{
public:
    static Config& getInstance();

    // void loadWords();
    // void saveWords() const;

    // void loadBooks();

    void setTextFileName(const std::string& fileName);    // читаемый файл
    const std::string getTextFileName() const;

    // const std::set<std::string>& getKnownWords() const;
    //    const std::set<std::string>& getIgnoredWords() const;
    //    const std::map<std::string, WordClass>& getWordsInfo() const;

    const std::string& getWordColorByType(const WordType& word);

    //    void addKnownWord(const WordClass& wordInfo);
    //    void addNewWordInfo(const WordClass& wordInfo
    //                        /*std::string &example, std::string &syn*/);

    void setCurrentBook(int id);

private:
    int currentBookId;
    std::string m_textFName;
    //    std::map<std::string, WordClass> m_knownWordsMap;

    //    std::set<std::string> m_ignoredWords;
    //    std::set<std::string> m_newIgnoredWords;

    //    std::map<std::string, WordClass> m_newWordsMap;

    // std::map<int, LibraryBook> m_books;

    Config();

    //  enum class WordType { Known, Unknown, Ignored };
    std::map<WordType, std::string> m_colors;
};

inline const std::string Config::getTextFileName() const { return m_textFName; }

// inline const std::set<std::string>& Config::getKnownWords()     const {
// return m_knownWords; };
// inline const std::set<std::string>& Config::getIgnoredWords() const
//{
//    return m_ignoredWords;
//}
// inline const std::map<std::string, WordClass>& Config::getWordsInfo() const
//{
//    return m_knownWordsMap;
//}

inline const std::string& Config::getWordColorByType(const WordType& word)
{
    return m_colors[word];
}
