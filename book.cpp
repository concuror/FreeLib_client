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

#include "book.h"
#include <QtCore>

using namespace freeLib;

Book::Book(QObject *parent) :
    QObject(parent)
{
    _name = NULL;
    _author = NULL;
    _extension = NULL;
    _added = NULL;
    _path = NULL;
    _id = 0;
}

Book::Book(const QVariantMap& data, QObject *parent):
    QObject(parent)
{
    _name = new QString(data.value("name").toString());
    _author = new QString(data.value("author").toString());
    _extension = new QString(data.value("extension").toString());
    _id = data.value("id").toInt();
    _added = new QDateTime(data.value("added_at").toDateTime());
    _path = NULL;
}

Book::Book(const Book& other):
    QObject(other.parent())
{
    _name = NULL;
    _author = NULL;
    _extension = NULL;
    _added = NULL;
    _path = NULL;
    _id = 0;
    *this = other;
}

void Book::clearData() {
    delete _name;
    delete _author;
    delete _extension;
    delete _added;
    delete _path;
}

void Book::setInfoFrom(const QVariantMap &data) {
    clearData();
    _name = new QString(data.value("name").toString());
    _author = new QString(data.value("author").toString());
    _extension = new QString(data.value("extension").toString());
    _added = new QDateTime(data.value("added_at").toDateTime());
}

int Book::updateInfo(const QVariantMap &data) {
    if (_id != data.value("id").toInt()) {
        return -1;
    }
    this->setInfoFrom(data);
    return 0;
}

QString *Book::name() const {
    return _name;
}

QString *Book::author() const {
    return _author;
}

QString *Book::extension() const {
    return _extension;
}

QString *Book::path() const {
    return _path;
}

int Book::id() const {
    return _id;
}

QDateTime *Book::addedAt() const {
    return _added;
}

QString Book::filename() const {
    QString filename(*_author);
    filename.append("_");
    filename.append(*_name);
    filename.append(*_extension);
    filename.replace(" ","_");
    return filename;
}

bool Book::operator ==(const Book& right) const {
    return right.id() == _id;
}

Book &Book::operator =(const Book& right) {
    if (this != &right) {
        clearData();
        _name = new QString(*right.name());
        _author = new QString(*right.author());
        _extension = new QString(*right.extension());
        _id = right.id();
        _added = new QDateTime(*right.addedAt());
        if (right.path() != NULL) {
            _path = new QString(*right.path());
        } else {
            _path = NULL;
        }
    }
    return *this;
}

Book::~Book() {
    clearData();
}
