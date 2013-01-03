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

#ifndef BOOK_H
#define BOOK_H

#include <QObject>

namespace freeLib {

class Book : public QObject
{
    Q_OBJECT

private:

    QString *_name;

    QString *_author;

    int _id;

    QDateTime *_added;

    QString *_extension;

public:

    explicit Book(QObject *parent = 0);

    explicit Book(const QVariantMap& data, QObject *parent = 0);

    Book(const Book& other);

    QString *name() const;

    QString *author() const;

    QString *extension() const;

    const int id() const;

    QDateTime *addedAt() const;

    QString filename() const;

    bool operator ==(const Book& right);

    Book& operator =(const Book& right);

    virtual ~Book();
    
signals:
    
public slots:
    
};

}
#endif // BOOK_H
