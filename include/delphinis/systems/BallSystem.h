#pragma once

#include "delphinis/ecs/System.h"

namespace delphinis {

class BallSystem : public System {
public:
    void update(World& world, float deltaTime) override;
};

} // namespace delphinis
