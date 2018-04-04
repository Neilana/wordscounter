#pragma once

#include <QAbstractListModel>

#include "WordItem.h"

Q_DECLARE_METATYPE(WordItem)

class SearchWordModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SearchResultRoles
    {
        IdRole = Qt::UserRole + 1,
        PartOfSpeechRole,
        DefinitionRole,
        ExamplesRole,
        SynonymsRole

    };

    SearchWordModel(QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    // custom methods
    void addWord(WordItem word);
    void resetAll();

private:
    QVector<WordItem> m_words;
};
