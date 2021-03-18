#include "section.h"
#include "line.h"

#include "mathematics.h"

namespace GraphGeometry {

bool Section::has(Point a) const
{
    Line l(this->start(), this->end());
    double max_x = std::fmax(_start.x(), _end.x());
    double min_x = std::fmin(_start.x(), _end.x());
    double max_y = std::fmax(_start.y(), _end.y());
    double min_y = std::fmin(_start.y(), _end.y());

    bool res = false;
    if (l.has(a))
    {
        bool x_ok = (min_x < a.x() || fuzzyCompare(min_x, a.x())) && (max_x > a.x() || fuzzyCompare(max_x, a.x()));
        bool y_ok = (min_y < a.y() || fuzzyCompare(min_y, a.y())) && (max_y > a.y() || fuzzyCompare(max_y, a.y()));
        res = x_ok && y_ok;
    }
    return res;
}

Point Section::intersection(const Section &other, bool *ok) const
{
    // Construct two lines
    Line l1(this->start(), this->end());
    Line l2(other.start(), other.end());
    bool found = false;
    Point linesIntersectionPoint = l1.intersection(l2, &found);
    if (found)
        found = this->has(linesIntersectionPoint) && other.has(linesIntersectionPoint);
    if (ok) *ok = found;
    return linesIntersectionPoint;
}
}
