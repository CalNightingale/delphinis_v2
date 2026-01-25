#pragma once

#include "delphinis/math/Vec2.h"

namespace delphinis {

struct PlayerMissile {
    Vec2 origin;      // Where the missile was fired from
    Vec2 target;      // Where the missile is headed
    float speed;      // Movement speed (pixels per second)
};

} // namespace delphinis
