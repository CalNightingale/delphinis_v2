#include "delphinis/systems/AISystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/AIController.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"
#include <cmath>

namespace delphinis {

void AISystem::update(World& world, float deltaTime) {
    for (Entity entity : world.getEntities()) {
        if (world.hasComponents<AIController, Transform, Velocity>(entity)) {
            const auto& ai = world.getComponent<AIController>(entity);
            const auto& transform = world.getComponent<Transform>(entity);
            auto& velocity = world.getComponent<Velocity>(entity);

            if (ai.ballToTrack != NULL_ENTITY && world.hasComponent<Transform>(ai.ballToTrack)) {
                const auto& ballTransform = world.getComponent<Transform>(ai.ballToTrack);

                float diff = ballTransform.position.y - transform.position.y;

                if (std::abs(diff) > 0.5f) {
                    velocity.velocity.y = (diff > 0 ? 1.0f : -1.0f) * ai.speed;
                } else {
                    velocity.velocity.y = 0.0f;
                }
            } else {
                velocity.velocity.y = 0.0f;
            }
        }
    }
}

} // namespace delphinis
