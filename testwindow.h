//    Copyright 2012 Andrii Titov
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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


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

    void buttonGetPressed();

    void buttonAddPressed();
    
public:
    void setUpInterface();

    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();
    
private:
    Ui::TestWindow *ui;
    freeLib::LibConnector *connector;
};

#endif // TESTWINDOW_H
