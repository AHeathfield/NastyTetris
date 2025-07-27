#pragma once

#include <cmath>
#include <string>

class Vector2
{
public:
    float x, y;

    // Constructors
    Vector2()
        : x(0.f), y(0.f)
    {}
    Vector2(float x, float y)
        : x(x), y(y)
    {}

    // Operator overloads
    Vector2 operator+(const Vector2& b) const
    {
        return Vector2(x + b.x, y + b.y);
    }
    
    Vector2 operator-(const Vector2& b) const
    {
        return Vector2(x - b.x, y - b.y);
    }

    // Scalar multiplication
    Vector2 operator*(const float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    // Scalar division
    Vector2 operator/(const float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    // Useful methods
    void zero()
    {
        x = 0.f;
        y = 0.f;
    }

    void invert()
    {
        x = -x;
        y = -y;
    }

    float getMagnitude() const
    {
        return std::sqrtf(x*x + y*y);
    }

    bool equals(const Vector2& other) const
    {
        return (x == other.x && y == other.y);
    }

    void normalize()
    {
        float magnitude = getMagnitude();
        if (magnitude > 0)
        {
            this->x = x / magnitude;
            this->y = y / magnitude;
        }
    }

    float getAngleRadians() const
    {
        return std::atanf(y/x);
    }

    std::string PrintPosition() const
    {
        std::string pos = "X = " + std::to_string(x) + ", Y = " + std::to_string(y);
        return pos;
    }
};
