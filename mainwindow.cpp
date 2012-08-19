#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Rasterizer *rasterizer, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rasterizer_(rasterizer),
    scene_(-100, -100, 200, 200)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene_);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->scale(50, 50);
    QObject::connect(ui->btnRasterize, SIGNAL(clicked()), this, SLOT(doRasterize()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doRasterize()
{
    bool ok;
    bool incorrect_values = false;

    double resX = ui->lineEdit_resolutionX->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_resolutionX->setText("Incorrect!");
    }
    double resY = ui->lineEdit_resolutionY->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_resolutionY->setText("Incorrect!");
    }
    double aX = ui->lineEdit_aX->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_aX->setText("Incorrect!");
    }
    double aY = ui->lineEdit_aY->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_aY->setText("Incorrect!");
    }
    double bX = ui->lineEdit_bX->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_bX->setText("Incorrect!");
    }
    double bY = ui->lineEdit_bY->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_bY->setText("Incorrect!");
    }
    double cX = ui->lineEdit_cX->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_cX->setText("Incorrect!");
    }
    double cY = ui->lineEdit_cY->text().toDouble(&ok);
    if (!ok) {
        incorrect_values = true;
        ui->lineEdit_cY->setText("Incorrect!");
    }

    if (incorrect_values) return;

    rasterizer_->setResolution(SizeF(resX, resY));
    rasterizer_->setTriangle(PointF(aX, aY), PointF(bX, bY), PointF(cX, cY));

    scene_.clear();

    QPen cellPen(Qt::lightGray);
    for (double i = 0.0; i < 100.0; i+= resX) {
        scene_.addLine(i, -100, i, 100, cellPen);
        scene_.addLine(-i, -100, -i, 100, cellPen);
    }
    for (double i = 0.0; i < 100.0; i+= resY) {
        scene_.addLine(-100, i, 100, i, cellPen);
        scene_.addLine(-100, -i, 100, -i, cellPen);
    }

    std::list<Point> points = rasterizer_->rasterize();

    for (std::list<Point>::iterator i = points.begin(); i != points.end(); ++i) {
        scene_.addRect(resX * i->x(), -resY * i->y(), resX, -resY);
    }

    scene_.addLine(aX, -aY, bX, -bY);
    scene_.addLine(aX, -aY, cX, -cY);
    scene_.addLine(bX, -bY, cX, -cY);
}
