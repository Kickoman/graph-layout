#ifndef POINT_H
#define POINT_H

namespace GraphGeometry {

class Point
{
public:
    Point();
    Point(double x, double y);

    double x() const;

    double y() const;

private:
    double _x{};
    double _y{};
};

}
#endif // POINT_H
