#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include <cmath>
#include <cstdlib>

inline bool fuzzyCompare(double a, double b)
{
    return abs(a - b) < 1e7;
}

#endif // MATHEMATICS_H
