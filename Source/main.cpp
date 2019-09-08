#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setStyle("Fusion");
    //QFontDatabase::addApplicationFont("://Ubuntu-R.ttf");
    //qApp->setFont(QFont("Ubuntu", 8, QFont::Normal, false));
    w.show();
    return a.exec();
}
