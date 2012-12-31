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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


#include "testwindow.h"
#include "ui_testwindow.h"
#include <QtGui>
#include <QtWidgets>

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    connector = NULL;
}

void TestWindow::setUpInterface() {

    QTextEdit *textEdit = new QTextEdit();
    QPushButton *quitButton = new QPushButton("&Quit");

    //QObject::connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(buttonReadAllPressed()));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(textEdit);
    layout->addWidget(quitButton);

    this->centralWidget()->setLayout(layout);

}

void TestWindow::buttonReadAllPressed() {
    if (connector == NULL) {
        connector = new freeLib::LibConnector(new QString("http://shrouded-beach-6392.herokuapp.com/"),this);
    }
    connector->fetchFrom(new QString("books/add"));
}

TestWindow::~TestWindow()
{
    delete ui;
}
