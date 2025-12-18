#pragma once

#include <cmath>

namespace delphinis {

struct Vec2 {
    float x, y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(float scalar) : x(scalar), y(scalar) {}

    // Vector addition
    Vec2 operator+(const Vec2& other) const {
        return Vec2{x + other.x, y + other.y};
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Vector subtraction
    Vec2 operator-(const Vec2& other) const {
        return Vec2{x - other.x, y - other.y};
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Scalar multiplication
    Vec2 operator*(float scalar) const {
        return Vec2{x * scalar, y * scalar};
    }

    Vec2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Component-wise multiplication
    Vec2 operator*(const Vec2& other) const {
        return Vec2{x * other.x, y * other.y};
    }

    // Scalar division
    Vec2 operator/(float scalar) const {
        return Vec2{x / scalar, y / scalar};
    }

    Vec2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Unary minus
    Vec2 operator-() const {
        return Vec2{-x, -y};
    }

    // Dot product
    float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    // Length (magnitude)
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Squared length (avoids sqrt for comparisons)
    float lengthSquared() const {
        return x * x + y * y;
    }

    // Normalize (returns unit vector)
    Vec2 normalize() const {
        float len = length();
        if (len > 0.0f) {
            return Vec2{x / len, y / len};
        }
        return Vec2{0.0f, 0.0f};
    }

    // Distance to another vector
    float distance(const Vec2& other) const {
        return (*this - other).length();
    }
};

// Scalar * Vector
inline Vec2 operator*(float scalar, const Vec2& vec) {
    return vec * scalar;
}

} // namespace delphinis
