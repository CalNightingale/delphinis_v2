#pragma once

namespace delphinis {

class World;

class System {
public:
    virtual ~System() = default;
    virtual void update(World& world, float deltaTime) = 0;
};

} // namespace delphinis
