#include "jsonabstraction.h"
#include "qt-json/json.h"

using namespace freeLib;

JSONAbstraction::JSONAbstraction() {
}

QVariant JSONAbstraction::parse(const QString &json, bool &success) {
#if QT_VERSION >= 0x050000
    QJsonParseError *parseError = new QJsonParseError();
    QByteArray arr(json.toLatin1());
    QJsonDocument doc = QJsonDocument::fromJson(arr, parseError);
    if (parseError->error > 0) {
        success = false;
        qDebug() << parseError->errorString();
        return QVariant::QVariant();
    }
    success = true;
    return doc.toVariant();
#else
    return QtJson::parse(json, success);
#endif
}

QByteArray JSONAbstraction::serialize(const QVariant &data, bool &success) {
#if QT_VERSION >= 0x050000
    QJsonDocument doc = QJsonDocument::fromVariant(data);
    return doc.toJson();
#else
    return QtJson::serialize(data, success);
#endif
}

JSONAbstraction::~JSONAbstraction() {

}
