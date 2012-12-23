#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include "libconnector.h"

class LibConnector;

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public slots:

    void buttonReadAllPressed();
    
public:
    void setUpInterface();

    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();
    
private:
    Ui::TestWindow *ui;
    freeLib::LibConnector *connector;
};

#endif // TESTWINDOW_H
