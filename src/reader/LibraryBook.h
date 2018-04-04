#pragma once

#include <QDebug>
#include <QString>

struct Section
{
    int m_position;
    QString m_title;
    int m_level;
};

class LibraryBook
{
public:
    // private:
    int m_id;
    QString m_author;
    QString m_title;
    QString m_fileName;
    QString m_md5;
    QString m_language;

    QMap<QString, QString> m_notes;
    QVector<Section> m_sections;

public:
    LibraryBook(int id = -1, const QString &author = "", const QString &title = "",
                const QString &fileName = "", const QString &md5 = "",
                const QString &language = "");

    const QString &getFileName() const;
    const QString &getMd5() const;
    // bool getHighlighting() { return m_highlightWords; }

    const QMap<QString, QString> &getNotes() const;
    const QVector<Section> &getSections() const;
};

inline const QString &LibraryBook::getFileName() const { return m_fileName; }
inline const QString &LibraryBook::getMd5() const { return m_md5; }
inline const QMap<QString, QString> &LibraryBook::getNotes() const { return m_notes; }
inline const QVector<Section> &LibraryBook::getSections() const { return m_sections; }
