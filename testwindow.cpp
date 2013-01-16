//    Copyright(c) 2012 Andrii Titov
//
//    This file is part of FreeLib.

//    FreeLib is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    FreeLib is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with FreeLib.  If not, see <http://www.gnu.org/licenses/>.


#include "testwindow.h"
#include "book.h"
#include "libconnector.h"
#include "librarymanager.h"
#include "book.h"

using namespace freeLib;

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setCentralWidget(new QWidget);
    connector = NULL;
}

void TestWindow::setUpInterface() {

    textEdit = new QTextEdit();
    textEdit->setReadOnly(true);

    localBooksTable = new QTableWidget(0, 4);

    QStringList headers;
    headers << "name" << "author" << "added" << "id";
    localBooksTable->setHorizontalHeaderLabels(headers);
    localBooksTable->setColumnHidden(3, true);

    QPushButton *quitButton = new QPushButton("&Quit");

    QPushButton *addButton = new QPushButton("&Add");

    QPushButton *getButton = new QPushButton("&Get");

    QPushButton *listButton = new QPushButton("&List");

    QObject::connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QObject::connect(listButton, SIGNAL(clicked()), this, SLOT(buttonReadAllPressed()));

    QObject::connect(getButton, SIGNAL(clicked()), this, SLOT(buttonGetPressed()));

    QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(buttonAddPressed()));

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(localBooksTable);
    layout->addWidget(textEdit);
    layout->addWidget(quitButton);

    QVBoxLayout *buttLayout = new QVBoxLayout();
    buttLayout->addWidget(addButton);
    buttLayout->addWidget(getButton);
    buttLayout->addWidget(listButton);

    mainLayout->addLayout(layout);
    mainLayout->addLayout(buttLayout);

    this->centralWidget()->setLayout(mainLayout);

}

void TestWindow::setUpConnector() {
    connector = new freeLib::LibConnector(new QString("http://bookserver.herokuapp.com/"),this);
    QObject::connect(connector,SIGNAL(replyArrived(QString,QString)),this,SLOT(responseArrived(QString,QString)));
}

void TestWindow::buttonReadAllPressed() {
    if (connector == NULL) {
        this->setUpConnector();
    }
    connector->fetchFrom( "books/list" );
}

void TestWindow::buttonGetPressed() {
    if (connector == NULL) {
        this->setUpConnector();
    }
    QList<QTableWidgetItem> selectedItems = localBooksTable->selectedItems();
    if (selectedItems.count() < 1) {
        QDebug << "None selected";
        return;
    }
    int selectedRow = selectedItems.at(0)->row();
    QString address("books/get/");
    QTableWidgetItem *item = localBooksTable->item(selectedRow,3);
    address.append(item->text());
    connector->fetchFrom( address );
}

void TestWindow::buttonAddPressed() {
    if (connector == NULL) {
        this->setUpConnector();
    }
    QString path = QFileDialog::getOpenFileName(this,
                                                "Choose a book");
    QString ext = path.split('.')[path.split('.').length() - 1];
    if (NULL == path) {
        return;
    }
    qDebug() << ext;
    QFile book( path );
    if (!book.open(QIODevice::ReadOnly)) {
        qDebug() << book.errorString();
        return;
    }
    QByteArray arr = book.readAll();
    book.close();

    QVariantMap map;
    map.insert("author", "Andrii");
    map.insert("name","Monkey");
    map.insert("extension",ext);

    connector->sendData("books/add", arr, map);
}

void TestWindow::refreshTable() {
    QSet<Book> *books = LibraryManager::instance()->getBooks();
    int row = localBooksTable->rowCount();
    QSet<Book>::const_iterator booksIter;
    for (booksIter = books->constBegin(); booksIter != books->constEnd(); ++booksIter, ++row) {
        localBooksTable->insertRow(row);
        QTableWidgetItem *item0 = new QTableWidgetItem(*(*booksIter).name());
        QTableWidgetItem *item1 = new QTableWidgetItem(*(*booksIter).author());
        QTableWidgetItem *item2 = new QTableWidgetItem((*booksIter).addedAt()->toString());
        QTableWidgetItem *item3 = new QTableWidgetItem( QString::number( (*booksIter).id() ) );
        localBooksTable->setItem(row,0,item0);
        localBooksTable->setItem(row,1,item1);
        localBooksTable->setItem(row,2,item2);
        localBooksTable->setItem(row,3,item3);
    }
}

void TestWindow::responseArrived(const QString& path, const QString &resp) {
    textEdit->setText(resp);
    if (path.contains("books/list")) {
        for (int i=localBooksTable->rowCount()-1; i >= 0; --i)
            localBooksTable->removeRow(i);
        this->refreshTable();
    }
    else if (path.contains("books/get")) {
        for (int i=localBooksTable->rowCount()-1; i >= 0; --i)
            localBooksTable->removeRow(i);
        this->refreshTable();
    }
}

TestWindow::~TestWindow()
{

}
