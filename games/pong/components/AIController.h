#pragma once

#include "../../../include/delphinis/ecs/Entity.h"

namespace delphinis {

struct AIController {
    float speed{7.0f};
    Entity ballToTrack{NULL_ENTITY};

    AIController() = default;
    AIController(float spd, Entity ball) : speed(spd), ballToTrack(ball) {}
};

} // namespace delphinis
