#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <vector>
#include <list>
#include <cmath>

class Point {
    int x_, y_;
public:
    Point(int x = 0, int y = 0) : x_(x), y_(y) {}
    Point(const Point& p) : x_(p.x_), y_(p.y_) {}
    void setX(int x) {x_ = x;}
    void setY(int y) {y_ = y;}
    int x() const {return x_;}
    int y() const {return y_;}
};

class PointF {
    double x_, y_;
public:
    PointF(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}
    PointF(const PointF& p) : x_(p.x_), y_(p.y_) {}
    void setX(double x) {x_ = x;}
    void setY(double y) {y_ = y;}
    double x() const {return x_;}
    double y() const {return y_;}
};

class SizeF {
    double width_, height_;
public:
    SizeF(double width = 0.0, double height = 0.0) : width_(width), height_(height) {}
    SizeF(const SizeF& s) : width_(s.width_), height_(s.height_) {}
    void setWidth(double width) {width_ = width;}
    void setHeight(double height) {height_ = height;}
    double width() const {return width_;}
    double height() const {return height_;}
};

class Rasterizer
{
    SizeF resolution_;
    PointF a_, b_, c_;

    int sign(double x)
    {
        if (fabs(x) < 1e-3) return 0;
        return x > 0.0 ? 1 : -1;
    }

    std::vector<PointF> sorted3(PointF, PointF, PointF);
    std::list<Point> rasterizeLine(PointF, PointF);

public:
    Rasterizer();
    Rasterizer(SizeF, PointF, PointF, PointF);

    void setResolution(SizeF);
    void setTriangle(PointF, PointF, PointF);

    std::list<Point> rasterize();
};

#endif // RASTERIZER_H
