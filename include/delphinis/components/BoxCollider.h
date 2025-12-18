#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct BoxCollider {
    Vec2 halfSize{0.5f, 0.5f};  // Half-extents for AABB
    bool isTrigger{false};

    BoxCollider() = default;
    BoxCollider(Vec2 size) : halfSize(size * 0.5f) {}
    BoxCollider(float w, float h) : halfSize(w * 0.5f, h * 0.5f) {}
};

} // namespace delphinis
