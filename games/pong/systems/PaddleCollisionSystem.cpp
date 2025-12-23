#include "PaddleCollisionSystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/BoxCollider.h"
#include "../../../include/delphinis/components/Velocity.h"
#include "../components/Ball.h"
#include "../components/PaddleCollider.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace delphinis {

void PaddleCollisionSystem::update(World& world, float deltaTime) {
    // Find all balls
    for (Entity ballEntity : world.getEntities()) {
        if (!world.hasComponents<Ball, Transform, BoxCollider, Velocity>(ballEntity)) {
            continue;
        }

        auto& ballTransform = world.getComponent<Transform>(ballEntity);
        const auto& ballCollider = world.getComponent<BoxCollider>(ballEntity);
        auto& ballVelocity = world.getComponent<Velocity>(ballEntity);

        // Check collision with all paddles
        for (Entity paddleEntity : world.getEntities()) {
            if (!world.hasComponents<PaddleCollider, Transform>(paddleEntity)) {
                continue;
            }

            const auto& paddleTransform = world.getComponent<Transform>(paddleEntity);
            const auto& paddleCollisionData = world.getComponent<PaddleCollider>(paddleEntity);

            // Check AABB collision (using PaddleCollider's halfSize)
            if (checkAABB(ballTransform, ballCollider, paddleTransform, paddleCollisionData)) {
                // Check if ball is actually approaching the paddle
                // If ball is moving away, don't reflect (prevents multiple reflections)
                Vec2 diff = ballTransform.position - paddleTransform.position;

                // If ball is to the right of paddle and moving right, it's leaving (don't reflect)
                // If ball is to the left of paddle and moving left, it's leaving (don't reflect)
                bool isApproaching = (diff.x > 0 && ballVelocity.velocity.x < 0) ||
                                     (diff.x < 0 && ballVelocity.velocity.x > 0);

                std::cout << "COLLISION DETECTED!" << std::endl;
                std::cout << "  Ball pos: (" << ballTransform.position.x << ", " << ballTransform.position.y << ")" << std::endl;
                std::cout << "  Ball vel: (" << ballVelocity.velocity.x << ", " << ballVelocity.velocity.y << ")" << std::endl;
                std::cout << "  Paddle pos: (" << paddleTransform.position.x << ", " << paddleTransform.position.y << ")" << std::endl;
                std::cout << "  Is approaching: " << (isApproaching ? "YES" : "NO") << std::endl;

                if (isApproaching) {
                    // Calculate position-based reflection
                    Vec2 newVelocity = calculatePaddleReflection(
                        ballVelocity.velocity,
                        ballTransform.position,
                        paddleTransform,
                        paddleCollisionData,
                        paddleCollisionData.maxReflectionAngle
                    );

                    std::cout << "  REFLECTING! New vel: (" << newVelocity.x << ", " << newVelocity.y << ")" << std::endl;

                    ballVelocity.velocity = newVelocity;

                    // Separate ball from paddle to prevent sticky collisions
                    float overlapX = (ballCollider.halfSize.x + paddleCollisionData.halfSize.x) - std::abs(diff.x);
                    if (overlapX > 0) {
                        ballTransform.position.x += (diff.x > 0 ? overlapX : -overlapX);
                    }
                } else {
                    std::cout << "  Ball already moving away, skipping reflection" << std::endl;
                }
            }
        }
    }
}

bool PaddleCollisionSystem::checkAABB(const Transform& t1, const BoxCollider& c1,
                                       const Transform& t2, const PaddleCollider& c2) const {
    Vec2 min1 = t1.position - c1.halfSize;
    Vec2 max1 = t1.position + c1.halfSize;
    Vec2 min2 = t2.position - c2.halfSize;
    Vec2 max2 = t2.position + c2.halfSize;

    return (min1.x <= max2.x && max1.x >= min2.x) &&
           (min1.y <= max2.y && max1.y >= min2.y);
}

Vec2 PaddleCollisionSystem::calculatePaddleReflection(const Vec2& ballVelocity,
                                                       const Vec2& ballPosition,
                                                       const Transform& paddleTransform,
                                                       const PaddleCollider& paddleCollider,
                                                       float maxAngleDegrees) const {
    // Calculate offset from paddle center (-1 to 1)
    float offsetY = ballPosition.y - paddleTransform.position.y;
    float normalizedOffset = offsetY / paddleCollider.halfSize.y;

    // Clamp to valid range
    normalizedOffset = std::max(-1.0f, std::min(1.0f, normalizedOffset));

    // Convert max angle to radians
    const float PI = 3.14159265359f;
    float maxAngleRad = maxAngleDegrees * PI / 180.0f;

    // Map normalized offset to angle (-maxAngle to +maxAngle)
    float angleRad = normalizedOffset * maxAngleRad;

    // Determine ball direction (left or right)
    float directionX = ballVelocity.x > 0 ? 1.0f : -1.0f;

    // Reverse direction (ball bounces back)
    directionX = -directionX;

    // Calculate new velocity components
    // For a given angle from horizontal: vx = cos(angle), vy = sin(angle)
    float newVelX = std::cos(angleRad) * directionX;
    float newVelY = std::sin(angleRad);

    // Return normalized direction (BallSystem will handle speed normalization)
    return Vec2{newVelX, newVelY};
}

} // namespace delphinis
