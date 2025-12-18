#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct Ball {
    float speed{6.0f};

    Ball() = default;
    Ball(float spd) : speed(spd) {}
};

} // namespace delphinis
