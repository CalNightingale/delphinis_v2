#include "BallSystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../components/Ball.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/Velocity.h"
#include "../constants/PongConstants.h"
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
                transform.position = BALL_RESET_POSITION;

                // Random direction (left or right, with some vertical component)
                float randomY = (rand() % BALL_RANDOM_Y_RANGE - BALL_RANDOM_Y_OFFSET) / BALL_RANDOM_Y_DIVISOR;
                velocity.velocity = Vec2{BALL_RESET_VELOCITY_X, randomY};
            }
            else if (transform.position.x > m_viewWidth / 2) {
                // Left player scored
                m_leftScore++;

                // Reset ball to center
                transform.position = BALL_RESET_POSITION;

                // Random direction (left or right, with some vertical component)
                float randomY = (rand() % BALL_RANDOM_Y_RANGE - BALL_RANDOM_Y_OFFSET) / BALL_RANDOM_Y_DIVISOR;
                velocity.velocity = Vec2{-BALL_RESET_VELOCITY_X, randomY};
            }

            // Maintain constant speed
            float currentSpeed = velocity.velocity.length();
            if (currentSpeed > BALL_MIN_SPEED_CHECK) {
                velocity.velocity = velocity.velocity.normalize() * ball.speed;
            }
        }
    }
}

} // namespace delphinis
