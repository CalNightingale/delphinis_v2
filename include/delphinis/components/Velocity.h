#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct Velocity {
    Vec2 velocity{0.0f, 0.0f};

    Velocity() = default;
    Velocity(Vec2 vel) : velocity(vel) {}
    Velocity(float x, float y) : velocity(x, y) {}
};

} // namespace delphinis
