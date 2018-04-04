/// @file LibraryDatabase.cpp

#include <QSqlError>
#include <QSqlQuery>

#include "LibraryDatabase.h"
#include "loggerDefine.h"

LibraryDatabase::LibraryDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "LibraryDB");
    m_db.setDatabaseName("../wordscounter_project/data/databases/LibraryDB.db3");

    if (!m_db.open())
    {
        WCNT_LOG("[DB] Error (LibraryDB): {}", m_db.lastError().text().toStdString());
    }
}
