#include "AISystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../components/AIController.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/Velocity.h"
#include "../constants/PongConstants.h"
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

                if (std::abs(diff) > AI_DEAD_ZONE) {
                    velocity.velocity.y = (diff > 0 ? AI_DIRECTION_MULTIPLIER : -AI_DIRECTION_MULTIPLIER) * ai.speed;
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
