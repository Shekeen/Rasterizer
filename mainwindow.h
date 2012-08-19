#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rasterizer.h"

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(Rasterizer *rasterizer, QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Rasterizer *rasterizer_;
    QGraphicsScene scene_;

private slots:
    void doRasterize();
};

#endif // MAINWINDOW_H
