#include "Vector.h"

namespace geometry {

Vector::Vector()
        {
            x = 0;
            y = 0;
            z = 0;
        }

Vector::Vector(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector Vector::operator+(const Vector& other)
{
    return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator-(const Vector& other)
{
    return Vector(x - other.x, y - other.y, z - other.z);
}

Vector Vector::operator*(double scalar)
{
    return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator/(double scalar)
{
    return Vector(x / scalar, y / scalar, z / scalar);
}

double Vector::dot(const Vector& other)
{
    return x * other.x + y * other.y + z * other.z;
}

Vector Vector::cross(const Vector& other)
{
    return Vector(y * other.z - z * other.y,
                  z * other.x - x * other.z,
                  x * other.y - y * other.x);
}

}