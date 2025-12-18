#include "delphinis/systems/CollisionSystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/BoxCollider.h"
#include "delphinis/components/Velocity.h"
#include <cmath>

namespace delphinis {

void CollisionSystem::update(World& world, float deltaTime) {
    const auto& entities = world.getEntities();

    // Check all pairs of entities for collisions
    for (size_t i = 0; i < entities.size(); i++) {
        Entity e1 = entities[i];
        if (!world.hasComponents<Transform, BoxCollider>(e1)) continue;

        for (size_t j = i + 1; j < entities.size(); j++) {
            Entity e2 = entities[j];
            if (!world.hasComponents<Transform, BoxCollider>(e2)) continue;

            const auto& t1 = world.getComponent<Transform>(e1);
            const auto& c1 = world.getComponent<BoxCollider>(e1);
            const auto& t2 = world.getComponent<Transform>(e2);
            const auto& c2 = world.getComponent<BoxCollider>(e2);

            if (checkAABB(t1, c1, t2, c2)) {
                // Collision detected - reflect velocity if entity has velocity component
                Vec2 normal = getCollisionNormal(t1, c1, t2, c2);

                if (world.hasComponent<Velocity>(e1)) {
                    auto& v1 = world.getComponent<Velocity>(e1);
                    Vec2 vel = v1.velocity;
                    v1.velocity = vel - normal * 2.0f * vel.dot(normal);
                }

                if (world.hasComponent<Velocity>(e2)) {
                    auto& v2 = world.getComponent<Velocity>(e2);
                    Vec2 vel = v2.velocity;
                    v2.velocity = vel - normal * 2.0f * vel.dot(normal);
                }
            }
        }
    }
}

bool CollisionSystem::checkAABB(const Transform& t1, const BoxCollider& c1,
                                 const Transform& t2, const BoxCollider& c2) const {
    Vec2 min1 = t1.position - c1.halfSize;
    Vec2 max1 = t1.position + c1.halfSize;
    Vec2 min2 = t2.position - c2.halfSize;
    Vec2 max2 = t2.position + c2.halfSize;

    return (min1.x <= max2.x && max1.x >= min2.x) &&
           (min1.y <= max2.y && max1.y >= min2.y);
}

Vec2 CollisionSystem::getCollisionNormal(const Transform& t1, const BoxCollider& c1,
                                          const Transform& t2, const BoxCollider& c2) const {
    Vec2 diff = t1.position - t2.position;

    // Find axis of least penetration
    float overlapX = (c1.halfSize.x + c2.halfSize.x) - std::abs(diff.x);
    float overlapY = (c1.halfSize.y + c2.halfSize.y) - std::abs(diff.y);

    if (overlapX < overlapY) {
        // Collision on X axis
        return Vec2{diff.x > 0 ? 1.0f : -1.0f, 0.0f};
    } else {
        // Collision on Y axis
        return Vec2{0.0f, diff.y > 0 ? 1.0f : -1.0f};
    }
}

} // namespace delphinis
