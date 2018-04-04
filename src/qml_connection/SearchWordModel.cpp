#include "SearchWordModel.h"

SearchWordModel::SearchWordModel(QObject *parent) {}

QHash<int, QByteArray> SearchWordModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "word";
    roles[PartOfSpeechRole] = "partOfSpeech";
    roles[DefinitionRole] = "definition";
    roles[ExamplesRole] = "example";
    roles[SynonymsRole] = "synonyms";
    return roles;
}

int SearchWordModel::rowCount(const QModelIndex &parent) const { return m_words.size(); }

QVariant SearchWordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= m_words.size()) return QVariant();

    const WordItem &word = m_words.at(index.row());

    if (role == IdRole) return word.m_id;
    if (role == PartOfSpeechRole) return word.m_partOfSpeech;
    if (role == DefinitionRole) return word.m_definition;
    if (role == ExamplesRole) return word.m_example;
    if (role == SynonymsRole) return word.m_synonyms;

    return QVariant();
}

void SearchWordModel::addWord(WordItem word)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_words.push_back(word);
    endInsertRows();
}

void SearchWordModel::resetAll()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_words.clear();
    endRemoveRows();
}
