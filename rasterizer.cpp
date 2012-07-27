#include "rasterizer.h"

#include <QVector>
#include <QtAlgorithms>
#include <cmath>

Rasterizer::Rasterizer() :
    resolution_(1.0, 1.0),
    a_(0.0, 0.0),
    b_(1.0, 0.0),
    c_(0.0, 1.0)
{
}

bool Rasterizer::qPointCompare(const QPointF& a, const QPointF& b)
{
    return a.y() < b.y();
}

QList<QPoint> Rasterizer::rasterizeLine(QPointF a, QPointF b)
{
    QList<QPoint> pixel_list;

    QPoint first((int)floor(a.x() / resolution_.width()), (int)floor(a.y() / resolution_.height()));

    qreal xcoord = a.x(), ycoord = a.y();
    const qreal xend = b.x(), yend = b.y();
    const qreal xstep = (xend - xcoord) / (max(xend - xcoord, yend - ycoord) * 10),
                ystep = (xend - xcoord) / (max(xend - xcoord, yend - ycoord) * 10);

    for (; xcoord < xend && ycoord < yend; xcoord += xstep, ycoord += ystep) {
        QPoint new_point((int)floor(xcoord / resolution_.width()),
                         (int)floor(ycoord / resolution_height()));
        if (pixel_list.empty() || pixel_list.last().x() != new_point.x() || pixel_list.last().y() != new_point.y()) {
            pixel_list.push_back(new_point);
        }
    }

    return pixel_list;
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

    QVector<QPoint> vertices;
    vertices.reserve(3);
    vertices << a_ << b_ << c_;
    qSort(vertices.begin(), vertices.end(), qPointCompare);
    QPointF divisor(vertices[0].x() + (vertices[2].x() - vertices[0].x()) * (vertices[1].y() - vertices[0].y()) / (vertices[2].y() - vertices[0].y()),
                    vertices[1].y());
    QPointF a1(vertices[0]), b1(vertices[1]), c1(divisor);
    QPointF a2(vertices[2]), b2(vertices[1]), c2(divisor);

    QList<QPoint> b1a1 = rasterizeLine(b1, a1);
    QList<QPoint> c1a1 = rasterizeLine(c1, a1);
    QList<QPoint>::iterator b1a1_iter = b1a1.begin(),
                            c1a1_iter = c1a1.begin();
    while (b1a1_iter != b1a1.end() && c1a1_iter != c1a1.end()) {
        int x1 = b1a1_iter->x(), x2 = c1a1_iter->x(), y = b1a1_iter->y();
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        while (b1a1_iter != b1a1.end() && b1a1_iter->y() == y) {
            if (b1a1_iter->x() < x1) x1 = b1a1_iter->x();
            if (b1a1_iter->x() > x2) x2 = b1a1_iter->x();
            b1a1_iter++;
        }
        while (c1a1_iter != c1a1.end() && c1a1_iter->y() == y) {
            if (c1a1_iter->x() < x1) x1 = c1a1_iter->x();
            if (c1a1_iter->x() > x2) x2 = c1a1_iter->x();
            c1a1_iter++;
        }
        for (int x = x1; x <= x2; x++) pixel_list.push_back(QPoint(x, y));
    }

    QList<QPoint> b2a2 = rasterizeLine(b2, a2);
    QList<QPoint> c2a2 = rasterizeLine(c2, a2);
    QList<QPoint>::iterator b2a2_iter = b2a2.begin(),
                            c2a2_iter = c2a2.begin();
    while (b2a2_iter != b2a2.end() && c2a2_iter != c2a2.end()) {
        int x1 = b2a2_iter->x(), x2 = c2a2_iter->x(), y = b2a2_iter->y();
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        while (b2a2_iter != b2a2.end() && b2a2_iter->y() == y) {
            if (b2a2_iter->x() < x1) x1 = b2a2_iter->x();
            if (b2a2_iter->x() > x2) x2 = b2a2_iter->x();
            b2a2_iter++;
        }
        while (c2a2_iter != c2a2.end() && c2a2_iter->y() == y) {
            if (c2a2_iter->x() < x1) x1 = c2a2_iter->x();
            if (c2a2_iter->x() > x2) x2 = c2a2_iter->x();
            c2a2_iter++;
        }
        for (int x = x1; x <= x2; x++) pixel_list.push_back(QPoint(x, y));
    }

    return pixel_list;
}
