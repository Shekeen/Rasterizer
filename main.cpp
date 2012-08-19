#include <QtGui/QApplication>
#include <QDebug>

#include "mainwindow.h"
#include "rasterizer.h"

int main(int argc, char *argv[])
{
    Rasterizer r;

    QApplication a(argc, argv);
    MainWindow w(&r);
    w.show();

    return a.exec();
}
