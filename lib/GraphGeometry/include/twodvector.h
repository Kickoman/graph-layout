#ifndef TWODVECTOR_H
#define TWODVECTOR_H

#include "point.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace GraphGeometry {

///
/// \brief Converts radians to degrees
/// \param rad angle in radians
/// \return the same angle in degrees
///
inline double rad_to_deg(double rad) { return (rad * 180) / M_PI; }
///
/// \brief Converts degrees to radians
/// \param deg angle in degrees
/// \return the same angle in radians
///
inline double deg_to_rad(double deg) { return (deg * M_PI) / 180; }

class TwoDVector
{
public:
    ///
    /// \brief Constructs zero-length vector
    ///
    TwoDVector();
    ///
    /// \brief Constructs a vector by point
    /// \param x X coordinate
    /// \param y Y coordinate
    ///
    TwoDVector(double x, double y);
    ///
    /// \brief Constructs a vector by two points.
    /// \param from Start point
    /// \param to Finish point
    ///
    TwoDVector(Point from, Point to);
    ///
    /// \brief Getter for vector's X coordinate
    /// \return x coordinate
    ///
    double x() const;
    ///
    /// \brief Getter for vector's Y coordinate
    /// \return y coordinate
    ///
    double y() const;
    ///
    /// \brief Overloaded operator for vectors addition
    /// \param other Another vector to be added to the current vector
    /// \return The sum of the current vector and the \a other vector
    ///
    TwoDVector &operator+=(GraphGeometry::TwoDVector other);
    ///
    /// \brief Overloaded operator for vectors addition
    /// \param other Another vector to be added to the current vector
    /// \return The sum of the current vector and the \a other vector
    ///
    TwoDVector operator+(GraphGeometry::TwoDVector other) const;
    ///
    /// \brief Returns "dot product" of two vectors
    /// \param other Another vector to be used in the dot product calculation
    /// \return The value of the dot product
    ///
    double dotProduct(GraphGeometry::TwoDVector other) const;
    ///
    /// \brief Getter for the vector magnitude (length)
    /// \return Magnitude of vector
    ///
    double magnitude() const;
    ///
    /// \brief Rotates the vector by the given angle in degrees
    /// \param degrees The angle for rotation
    /// \return New vector which is rotated by the given angle
    ///
    TwoDVector rotateDeg(double degrees) const;
    ///
    /// \brief Rotates the vector by the given angle in radians
    /// \param radians The angle for rotation
    /// \return New vector which is rotated by the given angle
    ///
    TwoDVector rotateRad(double radians) const;
    ///
    /// \brief Multiplies the vector by the number
    /// \param number A number for vector to be multiplied by
    /// \return New vector of the magnitude \a number size bigger than the previous one
    ///
    TwoDVector multiply(double number) const;
    ///
    /// \brief Divides the vector by the number
    /// \param number A number for vector to be divided by
    /// \return New vector of the magnitude \a number size smaller than the previous one
    ///
    TwoDVector divide(double number) const;
private:
    double _x = 0;
    double _y = 0;
};

TwoDVector operator*(GraphGeometry::TwoDVector v, double number);
TwoDVector operator/(GraphGeometry::TwoDVector v, double number);

}
#endif // TWODVECTOR_H
