#pragma once

#include <QString>

class WordItem
{
public:
    WordItem(const QString &word = "", const QString &partOfSpeech = "",
             const QString &definiton = "", const QString &example = "",
             const QString &synonyms = "")
        : m_id(word),
          m_partOfSpeech(partOfSpeech),
          m_definition(definiton),
          m_example(example),
          m_synonyms(synonyms)
    {
    }

    QString m_id;
    QString m_partOfSpeech;

    QString m_definition;
    QString m_example;
    QString m_synonyms;

    const QString getWord() const;
    const QString getDefinition() const;
};

inline const QString WordItem::getWord() const { return m_id; }
inline const QString WordItem::getDefinition() const { return m_definition; }
