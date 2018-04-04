#include "MySqlQueryModel.h"

#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

MySqlQueryModel::MySqlQueryModel(QObject *parent) : QSqlQueryModel(parent) {}

void MySqlQueryModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    QSqlQueryModel::setQuery(query, db);
    generateRoleNames();
}

void MySqlQueryModel::setQuery(const QSqlQuery &query)
{
    QSqlQueryModel::setQuery(query);
    generateRoleNames();
}

void MySqlQueryModel::generateRoleNames()
{
    m_roleNames.clear();
    for (int i = 0; i < record().count(); i++)
    {
        m_roleNames.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
}

QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

QHash<int, QByteArray> MySqlQueryModel::roleNames() const { return m_roleNames; }
