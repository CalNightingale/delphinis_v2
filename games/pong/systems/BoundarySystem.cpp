#include "BoundarySystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../../../include/delphinis/ecs/Entity.h"
#include "../../../include/delphinis/components/Transform.h"
#include "../../../include/delphinis/components/BoxCollider.h"
#include "../../../include/delphinis/components/Velocity.h"
#include "../components/PaddleBoundary.h"

namespace pong {

void BoundarySystem::update(delphinis::World& world, float deltaTime) {
    using namespace delphinis;

    const auto& entities = world.getEntities();

    for (Entity entity : entities) {
        if (!world.hasComponents<Transform, BoxCollider, PaddleBoundary>(entity)) {
            continue;
        }

        auto& transform = world.getComponent<Transform>(entity);
        const auto& collider = world.getComponent<BoxCollider>(entity);
        const auto& boundary = world.getComponent<PaddleBoundary>(entity);

        // Calculate paddle's current bounds
        float paddleTop = transform.position.y + collider.halfSize.y;
        float paddleBottom = transform.position.y - collider.halfSize.y;

        // Clamp paddle position to stay within boundaries
        if (paddleTop > boundary.maxY) {
            transform.position.y = boundary.maxY - collider.halfSize.y;
            // Stop upward movement
            if (world.hasComponent<Velocity>(entity)) {
                auto& velocity = world.getComponent<Velocity>(entity);
                if (velocity.velocity.y > 0.0f) {
                    velocity.velocity.y = 0.0f;
                }
            }
        }

        if (paddleBottom < boundary.minY) {
            transform.position.y = boundary.minY + collider.halfSize.y;
            // Stop downward movement
            if (world.hasComponent<Velocity>(entity)) {
                auto& velocity = world.getComponent<Velocity>(entity);
                if (velocity.velocity.y < 0.0f) {
                    velocity.velocity.y = 0.0f;
                }
            }
        }
    }
}

} // namespace pong
