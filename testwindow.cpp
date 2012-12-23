#include "testwindow.h"
#include "ui_testwindow.h"
#include <QtGui>
#include <QtWidgets>

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
}

void TestWindow::setUpInterface() {

    QTextEdit *textEdit = new QTextEdit();
    QPushButton *quitButton = new QPushButton("&Quit");

    QObject::connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(textEdit);
    layout->addWidget(quitButton);

    this->centralWidget()->setLayout(layout);

}

void TestWindow::buttonReadAllPressed() {
    if (connector == NULL) {
        connector = new freeLib::LibConnector(new QString(""),this);
    }
    connector->fetchFrom(new QString("all.json"));
}

TestWindow::~TestWindow()
{
    delete ui;
}
