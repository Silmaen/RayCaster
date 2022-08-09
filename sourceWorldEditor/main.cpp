/**
 * @file main.cpp
 * @author Silmaen
 * @date 09/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "we/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]){
    QApplication app(argc,argv);
    we::MainWindow windows;
    windows.show();
    return app.exec();
}
