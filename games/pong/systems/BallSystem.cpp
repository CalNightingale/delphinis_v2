#include "BallSystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../components/Ball.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/Velocity.h"
#include <cmath>

namespace delphinis {

void BallSystem::update(World& world, float deltaTime) {
    for (Entity entity : world.getEntities()) {
        if (world.hasComponents<Ball, Velocity>(entity)) {
            const auto& ball = world.getComponent<Ball>(entity);
            auto& velocity = world.getComponent<Velocity>(entity);

            // Maintain constant speed
            float currentSpeed = velocity.velocity.length();
            if (currentSpeed > 0.1f) {
                velocity.velocity = velocity.velocity.normalize() * ball.speed;
            }
        }
    }
}

} // namespace delphinis
