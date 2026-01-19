#include "WallCollisionSystem.h"
#include <delphinis/ecs/World.h>
#include <delphinis/components/Transform.h>
#include <delphinis/components/BoxCollider.h>
#include <delphinis/components/Velocity.h>
#include "components/Ball.h"
#include "components/Wall.h"

namespace delphinis {

WallCollisionSystem::WallCollisionSystem(AudioManager& audioManager, SoundId wallSound)
    : m_audioManager(audioManager)
    , m_wallSound(wallSound)
{
}

void WallCollisionSystem::update(World& world, float /*deltaTime*/) {
    // Find all balls
    for (Entity ballEntity : world.query<Ball, Transform, BoxCollider, Velocity>()) {
        const auto& ballTransform = world.getComponent<Transform>(ballEntity);
        const auto& ballCollider = world.getComponent<BoxCollider>(ballEntity);
        const auto& ballVelocity = world.getComponent<Velocity>(ballEntity);

        // Check collision with top/bottom walls
        for (Entity wallEntity : world.query<Wall, Transform, BoxCollider>()) {
            const auto& wall = world.getComponent<Wall>(wallEntity);

            // Only check top and bottom walls (not goals)
            if (wall.type != WallType::Top && wall.type != WallType::Bottom) {
                continue;
            }

            const auto& wallTransform = world.getComponent<Transform>(wallEntity);
            const auto& wallCollider = world.getComponent<BoxCollider>(wallEntity);

            // AABB collision check
            Vec2 ballMin = ballTransform.position - ballCollider.halfSize;
            Vec2 ballMax = ballTransform.position + ballCollider.halfSize;
            Vec2 wallMin = wallTransform.position - wallCollider.halfSize;
            Vec2 wallMax = wallTransform.position + wallCollider.halfSize;

            bool colliding = (ballMin.x <= wallMax.x && ballMax.x >= wallMin.x) &&
                             (ballMin.y <= wallMax.y && ballMax.y >= wallMin.y);

            if (colliding) {
                // Check if ball is moving towards the wall
                bool movingTowardsWall = (wall.type == WallType::Top && ballVelocity.velocity.y > 0) ||
                                          (wall.type == WallType::Bottom && ballVelocity.velocity.y < 0);

                if (movingTowardsWall) {
                    m_audioManager.playSound(m_wallSound);
                }
            }
        }
    }
}

} // namespace delphinis
