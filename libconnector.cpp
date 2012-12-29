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
    url.setUrl(*baseUrl + *page);
    qDebug() << url.toString();
    QNetworkRequest request(url);
    if (page->contains("books/list")) {
        networkManager->get(request);
    }
    else if (page->contains("books/add")) {
        QMap<QString,QVariant> map;
        map.insert("author","Andrii Titov");
        QByteArray arr("This is interesting book 2012");
        map.insert("book",arr);
//        map.insert("book","This is interesting book");
        QVariant tmp(map);
        QJsonDocument doc = QJsonDocument::fromVariant(tmp);
        qDebug() << "Variant:" << tmp;
        qDebug() << "Binary:" << doc.toJson();
        networkManager->put(request,doc.toJson());
    }
    delete page;
}

void LibConnector::requestFinishedWithReply(QNetworkReply *reply) {
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
    }
    else {
        QString path = reply->url().path();
        if (path.contains("books/list",Qt::CaseInsensitive)) {
            QJsonParseError *error = new QJsonParseError();
            QByteArray arr = reply->readAll();
            qDebug() << arr;
            QJsonDocument doc = QJsonDocument::fromJson(arr,error);
            if (error->error > 0) {
                qDebug() << error->errorString();
                return;
            }
            qDebug() << doc.toVariant();
        }
        else if (path.contains("books/add",Qt::CaseInsensitive)) {
            QByteArray arr = reply->readAll();
            qDebug() << arr;
        }
    }
}

LibConnector::~LibConnector() {
    delete baseUrl;
}
