#include "WordsDatabase.h"
#include "loggerDefine.h"

#include <QSqlError>
#include <QSqlQuery>

WordsDatabase::WordsDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "WordsDB");
    m_db.setDatabaseName("../wordscounter_project/data/databases/WordsDB.db3");
    if (!m_db.open())
    {
        WCNT_LOG("[DB] Error (WordsDB): {}", m_db.lastError().text().toStdString());
    }
}
