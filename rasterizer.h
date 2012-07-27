#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QPoint>
#include <QPointF>
#include <QSizeF>
#include <QList>

class Rasterizer
{
    QSizeF resolution_;
    QPointF a_, b_, c_;

    bool qPointCompare(QPointF, QPointF);
    QList<QPoint> rasterizeLine(QPointF, QPointF);

public:
    Rasterizer();
    Rasterizer(QSizeF, QPointF, QPointF, QPointF);

    void setResolution(QSizeF);
    void setTriangle(QPointF, QPointF, QPointF);

    QList<QPoint> rasterize();
};

#endif // RASTERIZER_H
