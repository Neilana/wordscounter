#pragma once

#include <QSqlDatabase>

/**
 *  @brief Provides connection to library database.
 */
class LibraryDatabase
{
private:
    QSqlDatabase m_db;
    LibraryDatabase();

public:
    static QSqlDatabase& getInstance()
    {
        static LibraryDatabase instance;
        return instance.m_db;
    }
};
