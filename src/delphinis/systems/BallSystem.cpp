#include "delphinis/systems/BallSystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/Ball.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"
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
