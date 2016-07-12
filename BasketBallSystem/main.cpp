#include "mainwindowlayout.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindowLayout w;
    MainWindow mw;
    //w.show();
    mw.show();

    return a.exec();
}
