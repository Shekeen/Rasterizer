#include "rasterizer.h"

#include <algorithm>
#include <cmath>

enum IntersectType { VERT, HORIZ };

Rasterizer::Rasterizer() :
    resolution_(1.0, 1.0),
    a_(0.1, 0.1),
    b_(0.9, 0.9),
    c_(1.9, -1.9)
{
}

std::vector<PointF> Rasterizer::sorted3(PointF a, PointF b, PointF c)
{
    std::vector<PointF> ans;
    ans.resize(3);

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

std::vector<Point>* Rasterizer::rasterizeLine(PointF a, PointF b)
{
    std::vector<Point>* pixel_list = new std::vector<Point>;

    PointF dir(b.x() - a.x(), b.y() - a.y());
    bool minus_x = a.x() > b.x();
    bool minus_y = a.y() > b.y();

    int startx = minus_x ? (int)std::floor(a.x() / resolution_.width())
                         : (int)std::ceil(a.x() / resolution_.width());
    int starty = minus_y ? (int)std::floor(a.y() / resolution_.height())
                         : (int)std::ceil(a.y() / resolution_.height());

    int endx = minus_x ? (int)std::ceil(b.x() / resolution_.width())
                       : (int)std::floor(b.x() / resolution_.width());
    int endy = minus_y ? (int)std::ceil(b.y() / resolution_.height())
                       : (int)std::floor(b.y() / resolution_.height());

    Point start_point((int)std::floor(a.x() / resolution_.width()),
                      (int)std::floor(a.y() / resolution_.height()));
    Point end_point((int)std::floor(b.x() / resolution_.width()),
                    (int)std::floor(b.y() / resolution_.height()));

    bool has_vertical_intersections = true;
    bool has_horizontal_intersections = true;

    if (minus_x && endx > startx || !minus_x && endx < startx) has_vertical_intersections = false;
    if (minus_y && endy > starty || !minus_y && endy < starty) has_horizontal_intersections = false;

    if (startx > endx) {
        int tmp = startx;
        startx = endx;
        endx = tmp;
    }
    std::vector<PointF> vertical_intersect;
    vertical_intersect.reserve((int)(std::abs(a.x() - b.x()) / resolution_.width()) + 1);
    if (has_vertical_intersections) {
        for (int x = startx; x <= endx; x++) {
            double xcoord = x * resolution_.width();
            double ycoord = a.y() + (xcoord - a.x()) * dir.y() / dir.x();
            vertical_intersect.push_back(PointF(xcoord, ycoord));
        }
    }

    if (starty > endy) {
        int tmp = starty;
        starty = endy;
        endy = tmp;
    }
    std::vector<PointF> horizontal_intersect;
    horizontal_intersect.reserve((int)(std::abs(a.y() - b.y()) / resolution_.height()) + 1);
    if (has_horizontal_intersections) {
        if (dir.x() * dir.y() >= 0) {
            for (int y = starty; y <= endy; y++) {
                double ycoord = y * resolution_.height();
                double xcoord = a.x() + (ycoord - a.y()) * dir.x() / dir.y();
                horizontal_intersect.push_back(PointF(xcoord, ycoord));
            }
        } else {
            for (int y = endy; y >= starty; y--) {
                double ycoord = y * resolution_.height();
                double xcoord = a.x() + (ycoord - a.y()) * dir.x() / dir.y();
                horizontal_intersect.push_back(PointF(xcoord, ycoord));
            }
        }
    }

    std::vector< std::pair<PointF, IntersectType> > intersections;
    intersections.reserve(vertical_intersect.size() + horizontal_intersect.size());
    std::vector<PointF>::iterator vert_iter = vertical_intersect.begin(),
                                horiz_iter = horizontal_intersect.begin();
    while (vert_iter != vertical_intersect.end() || horiz_iter != horizontal_intersect.end()) {
        if (vert_iter == vertical_intersect.end()) {
            intersections.push_back(std::make_pair(*horiz_iter, HORIZ));
            ++horiz_iter;
            continue;
        }
        if (horiz_iter == horizontal_intersect.end()) {
            intersections.push_back(std::make_pair(*vert_iter, VERT));
            ++vert_iter;
            continue;
        }
        if (vert_iter->x() < horiz_iter->x()) {
            intersections.push_back(std::make_pair(*vert_iter, VERT));
            ++vert_iter;
        } else {
            intersections.push_back(std::make_pair(*horiz_iter, HORIZ));
            ++horiz_iter;
        }
    }

    pixel_list->reserve(intersections.size() + 1);

    pixel_list->push_back(start_point);
    pixel_list->push_back(end_point);

    if (intersections.size() > 1) {
        std::vector< std::pair<PointF, IntersectType> >::iterator first_point = intersections.begin(),
                                                                second_point = ++(intersections.begin());
        for (; second_point != intersections.end(); ++first_point, ++second_point) {
            if (first_point->second == VERT) { //VERT; VERT and VERT; HORIZ
                int x = (int)std::floor(first_point->first.x() / resolution_.width());
                int y = (int)std::floor(first_point->first.y() / resolution_.height());
                pixel_list->push_back(Point(x, y));
            } else {
                if (second_point->second == VERT) { //HORIZ; VERT
                    int x = (int)std::floor(first_point->first.x() / resolution_.width());
                    int y = (int)std::floor(second_point->first.y() / resolution_.height());
                    pixel_list->push_back(Point(x, y));
                } else { //HORIZ; HORIZ
                    int x = (int)std::floor(first_point->first.x() / resolution_.width());
                    int y = std::min((int)std::floor(first_point->first.y() / resolution_.height()),
                                     (int)std::floor(second_point->first.y() / resolution_.height()));
                    pixel_list->push_back(Point(x, y));
                }
            }
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
    bool operator()(const Point& a, const Point& b)
    {
        return a.y() < b.y();
    }
};

std::vector<Point>* Rasterizer::rasterize()
{
    std::vector<Point>* pixel_list = new std::vector<Point>;

    std::vector<PointF> vertices = sorted3(a_, b_, c_);

    PointF divisor(vertices[0].x()
                    + (vertices[2].x() - vertices[0].x()) * (vertices[1].y() - vertices[0].y())
                    / (vertices[2].y() - vertices[0].y()),
                   vertices[1].y());
    PointF a1(vertices[0]), b1(vertices[1]), c1(divisor);
    PointF a2(vertices[2]), b2(vertices[1]), c2(divisor);

    std::vector<Point>* b1a1 = rasterizeLine(b1, a1);
    std::sort(b1a1->begin(), b1a1->end(), PointCompare());
    std::vector<Point>* c1a1 = rasterizeLine(c1, a1);
    std::sort(c1a1->begin(), c1a1->end(), PointCompare());
    std::vector<Point>::iterator b1a1_iter = b1a1->begin();
    std::vector<Point>::iterator c1a1_iter = c1a1->begin();
    while (b1a1_iter != b1a1->end() && c1a1_iter != c1a1->end()) {
        int x1 = b1a1_iter->x(), x2 = c1a1_iter->x(), y = b1a1_iter->y();
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        while (b1a1_iter != b1a1->end() && b1a1_iter->y() == y) {
            if (b1a1_iter->x() < x1) x1 = b1a1_iter->x();
            if (b1a1_iter->x() > x2) x2 = b1a1_iter->x();
            ++b1a1_iter;
        }
        while (c1a1_iter != c1a1->end() && c1a1_iter->y() == y) {
            if (c1a1_iter->x() < x1) x1 = c1a1_iter->x();
            if (c1a1_iter->x() > x2) x2 = c1a1_iter->x();
            ++c1a1_iter;
        }
        for (int x = x1; x <= x2; x++) pixel_list->push_back(Point(x, y));
    }

    std::vector<Point>* b2a2 = rasterizeLine(b2, a2);
    std::sort(b2a2->begin(), b2a2->end(), PointCompare());
    std::vector<Point>* c2a2 = rasterizeLine(c2, a2);
    std::sort(c2a2->begin(), c2a2->end(), PointCompare());
    std::vector<Point>::iterator b2a2_iter = b2a2->begin();
    std::vector<Point>::iterator c2a2_iter = c2a2->begin();
    while (b2a2_iter != b2a2->end() && c2a2_iter != c2a2->end()) {
        int x1 = b2a2_iter->x(), x2 = c2a2_iter->x(), y = b2a2_iter->y();
        if (x1 > x2) {
            int tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        while (b2a2_iter != b2a2->end() && b2a2_iter->y() == y) {
            if (b2a2_iter->x() < x1) x1 = b2a2_iter->x();
            if (b2a2_iter->x() > x2) x2 = b2a2_iter->x();
            ++b2a2_iter;
        }
        while (c2a2_iter != c2a2->end() && c2a2_iter->y() == y) {
            if (c2a2_iter->x() < x1) x1 = c2a2_iter->x();
            if (c2a2_iter->x() > x2) x2 = c2a2_iter->x();
            ++c2a2_iter;
        }
        for (int x = x1; x <= x2; x++) pixel_list->push_back(Point(x, y));
    }

    return pixel_list;
}
