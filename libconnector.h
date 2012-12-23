#ifndef LIBCONNECTOR_H
#define LIBCONNECTOR_H

#include <QtNetwork/QtNetwork>

namespace freeLib {

class LibConnector : public QObject
{
    Q_OBJECT

private:

    QString *baseUrl;

    QNetworkAccessManager *networkManager;

public:
    explicit LibConnector(QObject *parent = 0);

    explicit LibConnector(QString *baseUrl, QObject *parent = 0);

    void fetchFrom(QString *page);

    virtual ~LibConnector();

signals:



public slots:

    void requestFinishedWithReply(QNetworkReply *);

};

}



#endif // LIBCONNECTOR_H
