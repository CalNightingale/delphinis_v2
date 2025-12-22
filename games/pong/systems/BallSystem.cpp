#include "BallSystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../components/Ball.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/Velocity.h"
#include <cmath>
#include <cstdlib>

namespace delphinis {

void BallSystem::update(World& world, float deltaTime) {
    for (Entity entity : world.getEntities()) {
        if (world.hasComponents<Ball, Transform, Velocity>(entity)) {
            const auto& ball = world.getComponent<Ball>(entity);
            auto& transform = world.getComponent<Transform>(entity);
            auto& velocity = world.getComponent<Velocity>(entity);

            // Check if ball went off screen (scored)
            if (transform.position.x < -m_viewWidth / 2) {
                // Right player scored
                m_rightScore++;

                // Reset ball to center
                transform.position = Vec2{0.0f, 0.0f};

                // Random direction (left or right, with some vertical component)
                float randomY = (rand() % 200 - 100) / 50.0f;  // -2.0 to 2.0
                velocity.velocity = Vec2{5.0f, randomY};
            }
            else if (transform.position.x > m_viewWidth / 2) {
                // Left player scored
                m_leftScore++;

                // Reset ball to center
                transform.position = Vec2{0.0f, 0.0f};

                // Random direction (left or right, with some vertical component)
                float randomY = (rand() % 200 - 100) / 50.0f;  // -2.0 to 2.0
                velocity.velocity = Vec2{-5.0f, randomY};
            }

            // Maintain constant speed
            float currentSpeed = velocity.velocity.length();
            if (currentSpeed > 0.1f) {
                velocity.velocity = velocity.velocity.normalize() * ball.speed;
            }
        }
    }
}

} // namespace delphinis
