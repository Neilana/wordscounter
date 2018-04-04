#include "NetworkManager.h"

#include <QUrl>

#include "loggerDefine.h"

NetworkManager::NetworkManager(QObject *parent)
{
    m_appId = "35ad6c2d";
    m_appKey = "6a62f7710cdae1b936cf43e6e428a918";
    m_baseUrl = "https://od-api.oxforddictionaries.com/api/v1/";

    QObject::connect(&m_networkManager, SIGNAL(finished(QNetworkReply *)), this,
                     SLOT(replyRecieved(QNetworkReply *)));
}

void NetworkManager::sendSearchRequest(const QString &word)
{
    if (m_cachedResults.count(word) > 0)
    {
        WCNT_LOG("[NETWORK] Return search result from cache for word: {}",
                 word.toStdString());
        emit recievedEntryResultSignal(m_cachedResults[word]);
        return;
    }

    if (m_emptyPastSearches.contains(word)) return;

    // url parameters
    QUrlQuery query;

    query.addQueryItem("q", word);

    QUrl url("https://od-api.oxforddictionaries.com/api/v1/search/en");
    url.setQuery(query);

    QNetworkRequest networkRequest(url);

    networkRequest.setRawHeader("app_id", m_appId.toStdString().c_str());
    networkRequest.setRawHeader("app_key", m_appKey.toStdString().c_str());

    // send GET request when the button is clicked
    m_networkManager.get(networkRequest);
}

void NetworkManager::sendGetEntryRequest(const QString &word)
{
    if (m_cachedResults.count(word) > 0)
    {
        WCNT_LOG("[NETWORK] Return get entry result from cache for word: {}",
                 word.toStdString());
        ;
        emit recievedEntryResultSignal(m_cachedResults[word]);
        return;
    }

    QString urlStr = "https://od-api.oxforddictionaries.com/api/v1/entries/en/" + word;
    QUrl url(urlStr);

    QNetworkRequest networkRequest(url);
    networkRequest.setRawHeader("app_id", m_appId.toStdString().c_str());
    networkRequest.setRawHeader("app_key", m_appKey.toStdString().c_str());

    // send GET request when the button is clicked
    m_networkManager.get(networkRequest);
}

void NetworkManager::requestSearchResultsForWord(const QString &word)
{
    sendSearchRequest(word);
}

void NetworkManager::replyRecieved(QNetworkReply *reply)
{
    qDebug() << reply->request().url();
    QString urlStr = reply->request().url().toString();

    // if search request
    if (urlStr.startsWith("https://od-api.oxforddictionaries.com/api/v1/search/en"))
        getSearchReply(reply);
    else    // get entry request
        getEntryReply(reply);
}

void NetworkManager::getSearchReply(QNetworkReply *reply)
{
    // it can be a slot in your GUI window class
    // check for errors
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error: " << reply->error();
        m_networkManager.clearAccessCache();
    }
    else
    {
        // parse the reply JSON and display result in the UI
        QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();
        // QString fullName= jsonObject["results"].toString();

        if (jsonObject.isEmpty() || jsonObject["results"].toArray().isEmpty()) return;

        QJsonArray resultsArray = jsonObject["results"].toArray();
        QString resStr = resultsArray[0].toObject()["id"].toString();

        emit recievedSearchResultSignal(resStr);
        sendGetEntryRequest(resStr);
    }
    reply->deleteLater();
}

void NetworkManager::getEntryReply(QNetworkReply *reply)
{
    QVector<WordItem> resultWords;
    // it can be a slot in your GUI window class
    // check for errors
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error: " << reply->error();
        m_networkManager.clearAccessCache();
    }
    else
    {
        // parse the reply JSON and display result in the UI
        QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();
        if (jsonObject.isEmpty() || jsonObject["results"].toArray().isEmpty()) return;

        QJsonArray lexicalArray =
            jsonObject["results"].toArray()[0].toObject()["lexicalEntries"].toArray();

        QString word = jsonObject["results"].toArray()[0].toObject()["word"].toString();

        for (int i = 0; i < lexicalArray.size(); ++i)
        {
            QJsonObject lexicalObject = lexicalArray[i].toObject();
            QString partOfSpeech = lexicalObject["lexicalCategory"].toString();

            QJsonArray entries = lexicalObject["entries"].toArray();
            for (int j = 0; j < entries.size(); ++j)
            {
                QJsonObject entryObject = entries[j].toObject();
                QJsonArray senses = entryObject["senses"].toArray();
                for (int k = 0; k < senses.size(); ++k)
                {
                    QJsonObject senseObject = senses[k].toObject();

                    // definition
                    QString definition;
                    if (!senseObject["definitions"].toArray().isEmpty())
                        definition = senseObject["definitions"].toArray()[0].toString();

                    if (definition == "" &&
                        !senseObject["short_definitions"].toArray().isEmpty())
                        definition =
                            senseObject["short_definitions"].toArray()[0].toString();
                    // FIXME !!! Add croosrefs

                    // example
                    QString example;
                    if (!senseObject["examples"].toArray().isEmpty())
                    {
                        example = senseObject["examples"]
                                      .toArray()[0]
                                      .toObject()["text"]
                                      .toString();
                    }

                    WordItem bufWord{word, partOfSpeech, definition, example, ""};
                    resultWords.push_back(bufWord);
                    qDebug() << word << partOfSpeech << definition << example;
                }
            }
        }

        m_cachedResults[word] = resultWords;
        emit recievedEntryResultSignal(resultWords);
    }
    reply->deleteLater();
}
