#pragma once

#include "../../../include/delphinis/ecs/System.h"

namespace pong {

class BoundarySystem : public delphinis::System {
public:
    void update(delphinis::World& world, float deltaTime) override;
};

} // namespace pong
