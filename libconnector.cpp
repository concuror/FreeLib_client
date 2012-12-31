//    Copyright(c) 2012 Andrii Titov
//
//    This file is part of FreeLib.
//
//    FreeLib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    FreeLib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


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
        QFile book("/Users/concuror/Downloads/Abnett_Chuma_183366.mobi");
        if (!book.open(QIODevice::ReadOnly)) {
            qDebug() << book.errorString();
            return;
        }
        QByteArray arr = book.readAll();
        book.close();
        map.insert("book",arr.toBase64());
        QVariant tmp(map);
        QJsonDocument doc = QJsonDocument::fromVariant(tmp);
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
