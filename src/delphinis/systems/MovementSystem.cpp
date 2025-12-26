#include "delphinis/systems/MovementSystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"

namespace delphinis {

void MovementSystem::update(World& world, float deltaTime) {
    for (Entity entity : world.query<Transform, Velocity>()) {
        auto& transform = world.getComponent<Transform>(entity);
        const auto& velocity = world.getComponent<Velocity>(entity);

        transform.position = transform.position + velocity.velocity * deltaTime;
    }
}

} // namespace delphinis
