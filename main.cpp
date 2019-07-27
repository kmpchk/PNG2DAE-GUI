#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //a.setStyle("Fusion");
    w.show();

    QApplication::processEvents();

    return a.exec();
}
