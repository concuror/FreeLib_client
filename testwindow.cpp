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
#include "ui_testwindow.h"
#include "libconnector.h"
#include "librarymanager.h"
#include "book.h"

using namespace freeLib;

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    connector = NULL;
}

void TestWindow::setUpInterface() {

    textEdit = new QTextEdit();
    textEdit->setReadOnly(true);

    localBooksTable = new QTableWidget(0, 3);

    QStringList headers;
    headers << "name" << "author" << "added";
    localBooksTable->setHorizontalHeaderLabels(headers);

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
    connector->fetchFrom( "books/get/1" );
}

void TestWindow::buttonAddPressed() {
    if (connector == NULL) {
        this->setUpConnector();
    }
    connector->fetchFrom( "books/add" );
}

void TestWindow::responseArrived(const QString& path, const QString &resp) {
    textEdit->setText(resp);
    if (path.contains("books/list")) {
        QList<Book> *books = LibraryManager::instance()->getBooks();
        int row = localBooksTable->rowCount();
        QList<Book>::const_iterator booksIter;
        for (booksIter = books->constBegin(); booksIter != books->constEnd(); ++booksIter, ++row) {
            localBooksTable->insertRow(row);
            QTableWidgetItem *item0 = new QTableWidgetItem(*(*booksIter).name());
            QTableWidgetItem *item1 = new QTableWidgetItem(*(*booksIter).author());
            QTableWidgetItem *item2 = new QTableWidgetItem((*booksIter).addedAt()->toString());
            localBooksTable->setItem(row,0,item0);
            localBooksTable->setItem(row,1,item1);
            localBooksTable->setItem(row,2,item2);
        }

    }
}

TestWindow::~TestWindow()
{
    delete ui;
}
