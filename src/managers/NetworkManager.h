#pragma once

#include <QtNetwork>

#include "WordItem.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject* parent = 0);
    void sendSearchRequest(const QString& word);
    void sendGetEntryRequest(const QString& word);
    void requestSearchResultsForWord(const QString& word);

public slots:
    void replyRecieved(QNetworkReply* reply);
    void getSearchReply(QNetworkReply* reply);
    void getEntryReply(QNetworkReply* reply);

signals:
    void recievedSearchResultSignal(QString);
    void recievedEntryResultSignal(QVector<WordItem>);

private:
    QNetworkAccessManager m_networkManager;

    QMap<QString, QVector<WordItem>> m_cachedResults;
    QSet<QString> m_emptyPastSearches;

    QString m_appId;
    QString m_appKey;
    QString m_baseUrl;
};
