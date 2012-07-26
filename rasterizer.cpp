#include "rasterizer.h"

Rasterizer::Rasterizer() :
    resolution_(1.0, 1.0),
    a_(0.0, 0.0),
    b_(1.0, 0.0),
    c(0.0, 1.0)
{
}

Rasterizer::Rasterizer(QSizeF resolution,
                       QPointF a,
                       QPointF b,
                       QPointF c) :
    resolution_(resolution.width(), resolution.height()),
    a_(a.x(), a.y()),
    b_(b.x(), b.y()),
    c_(c.x(), c.y())
{
}

void Rasterizer::setResolution(QSizeF res)
{
    resolution_.setHeight(res.height());
    resolution_.setWidth(res.width());
}

void Rasterizer::setTriangle(QPointF a, QPointF b, QPointF c)
{
    a_.setX(a.x()); a_.setY(a.y());
    b_.setX(b.x()); b_.setY(b.y());
    c_.setX(c.x()); c_.setY(c.y());
}

QList<QPoint> Rasterizer::rasterize()
{
    QList<QPoint> pixel_list;
}
