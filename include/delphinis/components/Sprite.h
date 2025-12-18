#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct Vec3 {
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
};

struct Sprite {
    Vec2 size{1.0f, 1.0f};
    Vec3 color{1.0f, 1.0f, 1.0f};  // RGB color

    Sprite() = default;
    Sprite(Vec2 s, Vec3 c) : size(s), color(c) {}
};

} // namespace delphinis
