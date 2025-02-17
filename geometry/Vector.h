#pragma once

namespace geometry {

class Vector
{
    public:
        double x;
        double y;
        double z;

        Vector(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector operator+(const Vector& other)
        {
            return Vector(x + other.x, y + other.y, z + other.z);
        }

        Vector operator-(const Vector& other)
        {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        Vector operator*(double scalar)
        {
            return Vector(x * scalar, y * scalar, z * scalar);
        }

        Vector operator/(double scalar)
        {
            return Vector(x / scalar, y / scalar, z / scalar);
        }

        double dot(const Vector& other)
        {
            return x * other.x + y * other.y + z * other.z;
        }

        Vector cross(const Vector& other)
        {
            return Vector(y * other.z - z * other.y,
                          z * other.x - x * other.z,
                          x * other.y - y * other.x);
        }

};

}