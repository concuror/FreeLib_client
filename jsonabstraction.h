#ifndef JSONABSTRACTION_H
#define JSONABSTRACTION_H

#include <QtCore>

namespace freeLib {

class JSONAbstraction {

public:
    virtual ~JSONAbstraction();

    /**
     * Parse a JSON string
     *
     * \param json The JSON data
     * \param success The success of the parsing
     */
   static QVariant parse(const QString &json, bool &success);

    /**
    * This method generates a textual JSON representation
    *
    * \param data The JSON data generated by the parser.
    * \param success The success of the serialization
    *
    * \return QByteArray Textual JSON representation
    */
    static QByteArray serialize(const QVariant &data, bool &success);

private:
    JSONAbstraction();

};

}

#endif // JSONABSTRACTION_H