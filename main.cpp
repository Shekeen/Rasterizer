#include <QtGui/QApplication>
#include <QDebug>

#include "mainwindow.h"
#include "rasterizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Rasterizer r;
    std::list<Point> pixels = r.rasterize();
    for (std::list<Point>::iterator iter = pixels.begin(); iter != pixels.end(); ++iter) {
        int x = iter->x(), y = iter->y();
        qDebug() << x << y << ';';
    }

    return a.exec();
}
