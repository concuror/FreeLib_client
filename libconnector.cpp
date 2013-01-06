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
    else if (page.contains("books/add")) {

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        request.setRawHeader("author","Den Abnett");
        request.setRawHeader("name","Chuma");
        request.setRawHeader("extension",".mobi");

        QFile book("/Users/concuror/Downloads/Abnett_Chuma_183366.mobi");
        if (!book.open(QIODevice::ReadOnly)) {
            qDebug() << book.errorString();
            return;
        }
        QByteArray arr = book.readAll();
        book.close();

        qDebug() << "Length:" << arr.length() << "Binary:" << arr;
        request.setHeader(QNetworkRequest::ContentLengthHeader,arr.length());
        networkManager->put(request,arr);
    }
    else if (page.contains("books/get")) {
        networkManager->get(request);
    }
    else if (page.contains("books/download")) {
        networkManager->get(request);
    }
}

void LibConnector::requestFinishedWithReply(QNetworkReply *reply) {
    if (reply->error() > 0) {
        qDebug() << reply->errorString();
    }
    else {
        LibraryManager *manager = LibraryManager::instance();
        QString path = reply->url().path();
        QJsonParseError *parseError = new QJsonParseError();
        QByteArray arr = reply->readAll();
        QString reply(arr);
        //qDebug() << arr;
        if (path.contains("books/list",Qt::CaseInsensitive)) {
            QJsonDocument doc = QJsonDocument::fromJson(arr,parseError);
            if (parseError->error > 0) {
                qDebug() << parseError->errorString();
                return;
            }
            QVariantList list = doc.toVariant().toList();
            QVariantList::const_iterator bookIter;
            for (bookIter = list.constBegin(); bookIter != list.constEnd(); ++bookIter) {
                Book tmpBook((*bookIter).toMap());
                manager->addBook(tmpBook);
            }
        }
        else if (path.contains("books/add",Qt::CaseInsensitive)) {

        }
        else if (path.contains("books/get",Qt::CaseInsensitive)) {
            QJsonDocument doc = QJsonDocument::fromJson(arr,parseError);
            if (parseError->error > 0) {
                qDebug() << parseError->errorString();
                return;
            }
            QVariantMap map = doc.toVariant().toMap();

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

            filename = new QString( map.value("author").toString() );
            filename->append(map.value("name").toString());
            filename->append(map.value("extension").toString());
            filename->prepend("/Users/concuror/Downloads/");

            QString url("books/download/");
            url.append(map.value("id").toString());

            this->fetchFrom(url);

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
        emit replyArrived(path,reply);
    }
}

LibConnector::~LibConnector() {
    delete filename;
    delete baseUrl;
}
