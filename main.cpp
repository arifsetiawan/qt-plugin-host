#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("InfiniteSquares");
    QCoreApplication::setApplicationName("Plugin Host Demo");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
