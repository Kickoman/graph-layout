#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "twodvector.h"

namespace GraphGeometry {

///
/// \brief The Line class represents a straight line
///
class Line
{
public:
    Line();
    /// \brief Standard form line equation
    Line(double a, double b, double c);
    /// \brief Slop-intercept form
    Line(double k, double b);
    /// \brief By two points
    Line(Point a, Point b);
    /// \brief By the point and the direction vector
    Line(Point a, TwoDVector directionVector);

    // Standard form line equation
    ///
    /// \brief A argument of the standard form line equation
    /// \return A argument
    ///
    double A() const;
    ///
    /// \brief B argument of the standard form line equation
    /// \return B argument
    ///
    double B() const;
    ///
    /// \brief C argument of the standard form line equation
    /// \return C argument
    ///
    double C() const;

    // Slop-intercept form
    ///
    /// \brief k get the slope of the line (y = kx + b)
    /// The slope of a line, k, represents the change in y over the change in x.
    /// \return the line's slope
    ///
    double k() const;
    ///
    /// \brief b get the y-intercept of the line (y = kx + b)
    /// The value of the x where the line intercepts the y axis
    /// \return line's y-intercept
    ///
    double b() const;

    ///
    /// \brief Checks whether the point belongs to this line
    /// \param p point coordinates
    /// \return true if the point belongs to the line, false otherwise
    ///
    bool has(Point p) const;

    ///
    /// \brief Tries to find the intersection point between two lines
    /// \param other is the line to find intersection with
    /// \param ok optional parameter, which will be set as true if the point was found
    ///        or as false if the lines are parallel
    /// \return a point of intersection
    ///
    Point intersection(const Line &other, bool *ok = nullptr) const;

private:
    double _a = 0;
    double _b = 1;
    double _c = 0;
};

}

#endif // LINE_H
