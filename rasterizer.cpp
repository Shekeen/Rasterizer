#include "rasterizer.h"

#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer() :
    resolution_(1.0, 1.0),
    a_(0.0, 0.0),
    b_(1.0, 1.0),
    c_(2.0, -2.0)
{
}

std::vector<PointF> Rasterizer::sorted3(PointF a, PointF b, PointF c)
{
    std::vector<PointF> ans(3);

    if (a.y() < b.y()) {
        if (c.y() < a.y()) {
            ans[0] = c;
            ans[1] = a;
            ans[2] = b;
        } else if (c.y() > b.y()) {
            ans[0] = a;
            ans[1] = b;
            ans[2] = c;
        } else {
            ans[0] = a;
            ans[1] = c;
            ans[2] = b;
        }
    } else {
        if (c.y() < b.y()) {
            ans[0] = c;
            ans[1] = b;
            ans[2] = a;
        } else if (c.y() > a.y()) {
            ans[0] = b;
            ans[1] = a;
            ans[2] = c;
        } else {
            ans[0] = b;
            ans[1] = c;
            ans[2] = a;
        }
    }

    return ans;
}

std::list<Point> Rasterizer::rasterizeLine(PointF a, PointF b)
{
    std::list<Point> pixel_list;

    double xcoord = a.x(), ycoord = a.y();
    const double xend = b.x(), yend = b.y();
    const double denominator = fabs(xend - xcoord) > fabs(yend - ycoord) ?
                               fabs(xend - xcoord) :
                               fabs(yend - ycoord);
    const double xstep = (xend - xcoord) / (denominator * 10),
                 ystep = (xend - xcoord) / (denominator * 10);
    int xstep_sign = sign(xend - xcoord),
        ystep_sign = sign(yend - ycoord);
    for (; xstep_sign * xcoord < xstep_sign * xend && ystep_sign * ycoord < ystep_sign * yend;
           xcoord += xstep, ycoord += ystep) {
        Point new_point((int)floor(xcoord / resolution_.width()),
                         (int)floor(ycoord / resolution_.height()));
        if (pixel_list.empty() || pixel_list.back().x() != new_point.x() || pixel_list.back().y() != new_point.y()) {
            pixel_list.push_back(new_point);
        }
    }

    return pixel_list;
}

Rasterizer::Rasterizer(SizeF resolution,
                       PointF a,
                       PointF b,
                       PointF c) :
    resolution_(resolution.width(), resolution.height()),
    a_(a.x(), a.y()),
    b_(b.x(), b.y()),
    c_(c.x(), c.y())
{
}

void Rasterizer::setResolution(SizeF res)
{
    resolution_.setHeight(res.height());
    resolution_.setWidth(res.width());
}

void Rasterizer::setTriangle(PointF a, PointF b, PointF c)
{
    a_.setX(a.x()); a_.setY(a.y());
    b_.setX(b.x()); b_.setY(b.y());
    c_.setX(c.x()); c_.setY(c.y());
}

class PointCompare {
public:
    bool operator()(const PointF& a, const PointF& b)
    {
        return a.y() < b.y();
    }
};

std::list<Point> Rasterizer::rasterize()
{
    std::list<Point> pixel_list;

    std::vector<PointF> vertices = sorted3(a_, b_, c_);

    PointF divisor(vertices[0].x() + (vertices[2].x() - vertices[0].x()) * (vertices[1].y() - vertices[0].y()) / (vertices[2].y() - vertices[0].y()),
                    vertices[1].y());
    PointF a1(vertices[0]), b1(vertices[1]), c1(divisor);
    PointF a2(vertices[2]), b2(vertices[1]), c2(divisor);

    std::list<Point> b1a1 = rasterizeLine(b1, a1);
    std::list<Point> c1a1 = rasterizeLine(c1, a1);
    std::list<Point>::iterator b1a1_iter = b1a1.begin(),
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
            ++b1a1_iter;
        }
        while (c1a1_iter != c1a1.end() && c1a1_iter->y() == y) {
            if (c1a1_iter->x() < x1) x1 = c1a1_iter->x();
            if (c1a1_iter->x() > x2) x2 = c1a1_iter->x();
            ++c1a1_iter;
        }
        for (int x = x1; x <= x2; x++) pixel_list.push_back(Point(x, y));
    }

    std::list<Point> b2a2 = rasterizeLine(b2, a2);
    std::list<Point> c2a2 = rasterizeLine(c2, a2);
    std::list<Point>::iterator b2a2_iter = b2a2.begin(),
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
            ++b2a2_iter;
        }
        while (c2a2_iter != c2a2.end() && c2a2_iter->y() == y) {
            if (c2a2_iter->x() < x1) x1 = c2a2_iter->x();
            if (c2a2_iter->x() > x2) x2 = c2a2_iter->x();
            ++c2a2_iter;
        }
        for (int x = x1; x <= x2; x++) pixel_list.push_back(Point(x, y));
    }

    return pixel_list;
}
