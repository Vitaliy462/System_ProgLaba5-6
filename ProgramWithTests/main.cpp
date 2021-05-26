#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w("localhost",2323);
    w.show();
    return a.exec();
}
