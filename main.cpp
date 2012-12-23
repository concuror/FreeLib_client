#include "testwindow.h"
#include <QApplication>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow w;

    w.setUpInterface();

    w.show();

    return a.exec();
}
