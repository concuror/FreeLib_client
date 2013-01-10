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


#ifndef LIBCONNECTOR_H
#define LIBCONNECTOR_H

#include <QtNetwork/QtNetwork>

namespace freeLib {

class LibConnector : public QObject
{
    Q_OBJECT

private:

    QString *baseUrl;

    QString *filename;

    QNetworkAccessManager *networkManager;

public:
    explicit LibConnector(QObject *parent = 0);

    explicit LibConnector(QString *baseUrl, QObject *parent = 0);

    void fetchFrom(const QString& page);

    void sendData(const QString &page, const QByteArray &info, const QVariantMap &params);

    virtual ~LibConnector();

signals:

    void replyArrived(const QString& path, const QString& reply);

public slots:

    void requestFinishedWithReply(QNetworkReply *);

};

}



#endif // LIBCONNECTOR_H
