#pragma once

#include "delphinis/ecs/World.h"
#include <vector>

namespace delphinis {

class PlayerMissileSystem {
public:
    void update(World& world, float deltaTime);

    // Returns missiles that reached their targets this frame
    const std::vector<Entity>& getArrivedMissiles() const { return m_arrivedMissiles; }

private:
    std::vector<Entity> m_arrivedMissiles;
};

} // namespace delphinis
