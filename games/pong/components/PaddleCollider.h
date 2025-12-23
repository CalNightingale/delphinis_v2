#pragma once

#include "../../../include/delphinis/math/Vec2.h"

namespace delphinis {

struct PaddleCollider {
    Vec2 halfSize{0.4f, 2.0f};                    // Collision box half-extents
    float maxReflectionAngle{60.0f};              // Max angle in degrees from horizontal

    PaddleCollider() = default;
    PaddleCollider(float maxAngle) : maxReflectionAngle(maxAngle) {}
    PaddleCollider(Vec2 size, float maxAngle) : halfSize(size), maxReflectionAngle(maxAngle) {}
};

} // namespace delphinis
