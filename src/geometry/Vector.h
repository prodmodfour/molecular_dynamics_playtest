#pragma once

namespace geometry {

typedef struct Vector
{
        double x;
        double y;
        double z;

        Vector();
        Vector(double x, double y, double z);

        Vector operator+(const Vector& other);
        Vector operator-(const Vector& other);
        Vector operator*(double scalar);
        Vector operator/(double scalar);
        double dot(const Vector& other);
        Vector cross(const Vector& other);
 
} Vector;

}