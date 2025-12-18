#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct Transform {
    Vec2 position{0.0f, 0.0f};
    float rotation{0.0f};  // Radians
    Vec2 scale{1.0f, 1.0f};

    Transform() = default;
    Transform(Vec2 pos) : position(pos) {}
    Transform(float x, float y) : position(x, y) {}
};

} // namespace delphinis
