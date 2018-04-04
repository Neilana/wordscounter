/// @file WordsDatabase.h

#pragma once

#include <QSqlDatabase>

/**
 *  @brief Provides connection to dictionary database.
 */
class WordsDatabase
{
private:
    QSqlDatabase m_db;
    WordsDatabase();

public:
    static QSqlDatabase& getInstance()
    {
        static WordsDatabase instance;
        return instance.m_db;
    }
};
