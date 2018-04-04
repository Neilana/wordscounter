#include "LibraryBook.h"

#include <QCryptographicHash>
#include <QFile>

#include <string>

LibraryBook::LibraryBook(int id, const QString &author, const QString &title,
                         const QString &fileName, const QString &md5, const QString &lang)
    : m_id(id),
      m_author(author),
      m_title(title),
      m_fileName(fileName),
      m_md5(md5),
      m_language(lang)
{
}
