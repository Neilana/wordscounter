#pragma once

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QVector>

// forward declaration
class Core;

class QmlCppInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pageIndex READ getPageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(int percents READ getPercents NOTIFY percentsChanged)
    Q_PROPERTY(bool bookOpened READ isBookOpened NOTIFY bookOpenedChanged)

    Q_PROPERTY(QString openedNote READ getOpenedNote NOTIFY openedNoteChanged)
    Q_PROPERTY(QString programStatus READ getProgramStatus NOTIFY programStatusChanged)
    Q_PROPERTY(QString searchedWord READ getSearchedWord NOTIFY searchedWordChanged)

    //    Q_PROPERTY(int lastEndPos READ getLastEndPos NOTIFY lastEndPosChanged)
    //    Q_PROPERTY(int currentEndPos READ getCurrentEndPos NOTIFY currentEndPosChanged)

public:
    QmlCppInterface(QObject* parent = 0);
    virtual ~QmlCppInterface();

    // core
    void setCore(Core* core);
    void setPageIndex(int i);
    void setBookOpened(bool value);

    // book manager
    Q_INVOKABLE void openBookById(int id);
    Q_INVOKABLE void deleteBookById(int id);
    Q_INVOKABLE void addNewBookByFilename(QUrl fileName);

    Q_INVOKABLE void setPageSize(int width, int height);
    Q_INVOKABLE void loadPageByPercents(QString percentsStr);
    Q_INVOKABLE int getPageIndex();
    Q_INVOKABLE int getPercents();
    Q_INVOKABLE QString getOpenedNote() const;
    Q_INVOKABLE QString getProgramStatus() const;
    Q_INVOKABLE QString getSearchedWord() const;
    Q_INVOKABLE int isBookOpened();

    // hello animal
    Q_INVOKABLE QString getHelloAnimalImagePath(int id) const;
    Q_INVOKABLE QString getHelloAnimalName(int id) const;
    Q_INVOKABLE int generateHelloAnimalNumber();

    // text clicks
    Q_INVOKABLE bool isLink(const QString& htmlStr);
    Q_INVOKABLE bool isNote(const QString& htmlStr);
    Q_INVOKABLE bool isImage(const QString& htmlStr);
    Q_INVOKABLE QString extractLink(const QString& htmlStr);
    Q_INVOKABLE void wordClicked(const QString& word);
    Q_INVOKABLE void addWord(const QString& word, const QString& definition,
                             const QString& example);
    Q_INVOKABLE void contentClicked(int index);
    Q_INVOKABLE void searchedWordClicked(int index);

    Q_INVOKABLE QString countAllUnknownWordsByBookId(int id);

    Q_INVOKABLE void closeProgram();

    int m_currentPageIndex;

public slots:
    void updateNotes(QMap<QString, QString> notes);
    void changeProgramStatus(QString);
    void updateSearchedWord(QString);
    void clearProgramStatus();

signals:
    // to qml
    void pageIndexChanged();
    void percentsChanged();
    void bookOpenedChanged();
    void openedNoteChanged();
    void programStatusChanged();
    void searchedWordChanged();

    // to core/rm
    void setPageSizeSignal(int width, int height, int index);
    void loadPageByPercentsSignal(QString percentsStr);
    void loadFromContentsTable(int);
    void deleteBookByIdSignal(int id);
    void addNewBookByFilenameSignal(QString);
    void openLink();

private:
    void fillHelloAnimalsVector();

    struct HelloAnimal
    {
        QString m_fileName;
        QString m_name;
    };

    QVector<HelloAnimal> m_helloAnimals;

    Core* m_core;
    bool m_bookOpened;
    int m_percents;
    QString m_searchedWord;
    QString m_programStatus;
    QMap<QString, QString> m_notes;
    QString m_openedNote;

    QTimer* m_timer;
};
