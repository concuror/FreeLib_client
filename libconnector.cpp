#include "libconnector.h"

using namespace freeLib;

LibConnector::LibConnector(QObject *parent) :
    QObject(parent)
{
    baseUrl = NULL;
    networkManager = NULL;
}

LibConnector::LibConnector (QString *BaseUrl, QObject *parent) :
    QObject(parent)
{
    baseUrl = BaseUrl;
    networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinishedWithReply(QNetworkReply*)));
}

void LibConnector::fetchFrom(QString *page) {
    QUrl url;
    url.setUrl(baseUrl->append(page));
    QNetworkRequest request(url);
    networkManager->get(request);
    delete page;
}

void LibConnector::requestFinishedWithReply(QNetworkReply *reply) {
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
    }
    else {
        QString path = reply->url().path();
        if (path.contains("",Qt::CaseInsensitive)) {
            QJsonParseError *error = new QJsonParseError();
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(),error);
            if (error->error > 0) {
                qDebug() << error->errorString();
                return;
            }
            qDebug() << doc.toVariant().toString();
        }
    }
}

LibConnector::~LibConnector() {
    delete baseUrl;
}
