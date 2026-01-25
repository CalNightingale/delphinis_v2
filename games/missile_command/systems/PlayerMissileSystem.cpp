#include "PlayerMissileSystem.h"
#include "../components/PlayerMissile.h"
#include "../constants/MissileCommandConstants.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Sprite.h"

namespace delphinis {

void PlayerMissileSystem::update(World& world, float deltaTime) {
    m_arrivedMissiles.clear();

    for (Entity entity : world.query<PlayerMissile, Transform>()) {
        auto& transform = world.getComponent<Transform>(entity);
        const auto& missile = world.getComponent<PlayerMissile>(entity);

        // Calculate direction to target
        Vec2 toTarget = missile.target - transform.position;
        float distance = toTarget.length();

        // Check if we've arrived at the target
        float moveDistance = missile.speed * deltaTime;
        if (distance <= moveDistance) {
            // Move to exact target position
            transform.position = missile.target;
            m_arrivedMissiles.push_back(entity);
        } else {
            // Move toward target
            Vec2 direction = toTarget.normalize();
            transform.position += direction * moveDistance;
        }
    }

    // Destroy arrived missiles (they will spawn explosions in ExplosionSystem)
    for (Entity entity : m_arrivedMissiles) {
        world.destroyEntity(entity);
    }
}

} // namespace delphinis
