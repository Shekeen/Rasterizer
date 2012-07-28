#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <vector>
#include <list>

class Point {
    int x_, y_;
public:
    Point(int x, int y) : x_(x), y_(y) {}
    void setX(int x) {x_ = x;}
    void setY(int y) {y_ = y;}
    int x() const {return x_;}
    int y() const {return y_;}
};

class PointF {
    double x_, y_;
public:
    PointF(double x, double y) : x_(x), y_(y) {}
    void setX(double x) {x_ = x;}
    void setY(double y) {y_ = y;}
    double x() const {return x_;}
    double y() const {return y_;}
};

class SizeF {
    double width_, height_;
public:
    SizeF(double width, double height) : width_(width), height_(height) {}
    void setWidth(double width) {width_ = width;}
    void setHeight(double height) {height_ = height;}
    double width() const {return width_;}
    double height() const {return height_;}
};

class Rasterizer
{
    SizeF resolution_;
    PointF a_, b_, c_;

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
