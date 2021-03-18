#include "twodvector.h"

namespace GraphGeometry {

TwoDVector::TwoDVector() = default;
TwoDVector::TwoDVector(double x, double y) : _x(x), _y(y) {}
TwoDVector::TwoDVector(Point from, Point to)
    : _x(to.x() - from.x()), _y(to.y() - from.y()) {}

double TwoDVector::x() const { return _x; }
double TwoDVector::y() const { return _y; }

TwoDVector &TwoDVector::operator+=(GraphGeometry::TwoDVector other)
{
    _x += other._x;
    _y += other._y;
    return *this;
}
TwoDVector TwoDVector::operator+(GraphGeometry::TwoDVector other) const
{
    TwoDVector v;
    v._x = this->_x + other._x;
    v._y = this->_y + other._y;
    return v;
}

double TwoDVector::dotProduct(GraphGeometry::TwoDVector other) const
{
    return this->_x * other._x + this->_y * other._y;
}

double TwoDVector::magnitude() const
{
    return sqrt(_x * _x + _y * _y);
}

TwoDVector TwoDVector::rotateDeg(double degrees) const
{
    double radians = deg_to_rad(degrees);
    return this->rotateRad(radians);
}
TwoDVector TwoDVector::rotateRad(double radians) const
{
    double x2 = cos(radians) * _x - sin(radians) * _y;
    double y2 = sin(radians) * _x + cos(radians) * _y;
    return {x2, y2};
}

TwoDVector TwoDVector::multiply(double number) const
{
    double x2 = _x * number;
    double y2 = _y * number;
    return {x2, y2};
}

TwoDVector TwoDVector::divide(double number) const
{
    double x2 = _x / number;
    double y2 = _y / number;
    return {x2, y2};
}

TwoDVector operator*(GraphGeometry::TwoDVector v, double number)
{
    return v.multiply(number);
}

TwoDVector operator/(GraphGeometry::TwoDVector v, double number)
{
    return v.divide(number);
}

}
