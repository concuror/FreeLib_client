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
//    along with FreeLib.  If not, see <http://www.gnu.org/licenses/>.


#include "libconnector.h"
#include "librarymanager.h"
#include "book.h"
#include "jsonabstraction.h"

using namespace freeLib;

LibConnector::LibConnector(QObject *parent) :
    QObject(parent)
{
    baseUrl = NULL;
    networkManager = NULL;
    filename = NULL;
}

LibConnector::LibConnector (QString *BaseUrl, QObject *parent) :
    QObject(parent)
{
    baseUrl = BaseUrl;
    networkManager = new QNetworkAccessManager(this);
    filename = NULL;
    QObject::connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinishedWithReply(QNetworkReply*)));
}

void LibConnector::fetchFrom(const QString &page) {
    QUrl url;
    url.setUrl(*baseUrl + page);
    QNetworkRequest request(url);
    if (page.contains("books/list")) {
        networkManager->get(request);
    }
    else if (page.contains("books/get")) {
        networkManager->get(request);
    }
    else if (page.contains("books/download")) {
        networkManager->get(request);
    }
}

void LibConnector::sendData(const QString &page, const QByteArray &info, const QVariantMap &params) {
    QUrl url;
    url.setUrl(*baseUrl + page);
    QNetworkRequest request(url);
    if (page.contains("books/add")) {

        qDebug() << params;

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        request.setRawHeader("author",params["author"].toByteArray());
        request.setRawHeader("name",params["name"].toByteArray());
        request.setRawHeader("extension",params["extension"].toByteArray());

        request.setHeader(QNetworkRequest::ContentLengthHeader,info.length());
        networkManager->put(request,info);

    }
}

void LibConnector::requestFinishedWithReply(QNetworkReply *reply) {
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
        return;
    }
    LibraryManager *manager = LibraryManager::instance();
    QString path = reply->url().path();
    bool parseSuccess;
    QByteArray arr = reply->readAll();
    QString data(arr);
    if (path.contains("books/list",Qt::CaseInsensitive)) {
        QVariant json = JSONAbstraction::parse(data, parseSuccess);
        if (!parseSuccess) {
            return;
        }
        QVariantList list = json.toList();
        QVariantList::const_iterator bookIter;
        for (bookIter = list.constBegin(); bookIter != list.constEnd(); ++bookIter) {
            Book tmpBook((*bookIter).toMap());
            manager->addBook(tmpBook);
        }
    }
    else if (path.contains("books/add",Qt::CaseInsensitive)) {

    }
    else if (path.contains("books/get",Qt::CaseInsensitive)) {
        QVariant json = JSONAbstraction::parse(data, parseSuccess);
        if (!parseSuccess) {
            return;
        }
        QVariantMap map = json.toMap();

        QSet<Book> *books = LibraryManager::instance()->getBooks();
        QSet<Book>::const_iterator booksIter;
        Book tmp;
        int id = map.value("id").toInt();
        for (booksIter = books->constBegin(); booksIter != books->constEnd(); ++booksIter) {
            if ( (*booksIter).id() == id) {
                tmp = *booksIter;
                break;
            }
        }

        if (tmp.id() != 0) {
            books->remove(tmp);
            tmp.updateInfo(map);
            books->insert(tmp);
        }

        filename = new QString( tmp.filename() );

//        QString url("books/download/");
//        url.append(map.value("id").toString());

//        this->fetchFrom(url);

    }
    else if (path.contains("books/download",Qt::CaseInsensitive)) {
        QFile book(*filename);
        if (!book.open(QIODevice::WriteOnly)) {
            qDebug() << book.errorString();
        }
        else {
            book.write(arr);
            book.close();
        }
        delete filename;
        filename = NULL;
    }
    emit replyArrived(path,data);
}

LibConnector::~LibConnector() {
    delete filename;
    delete baseUrl;
}
