#include "MouseInputSystem.h"
#include "../components/MissileBattery.h"
#include "../components/PlayerMissile.h"
#include "../constants/MissileCommandConstants.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Sprite.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <limits>

namespace delphinis {

MouseInputSystem::MouseInputSystem(InputManager& inputManager, PixelCanvas& pixelCanvas)
    : m_inputManager(inputManager)
    , m_pixelCanvas(pixelCanvas)
{
}

void MouseInputSystem::update(World& world, float deltaTime) {
    (void)deltaTime;

    if (m_inputManager.isMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        Vec2 clickPos = m_inputManager.getMousePosition(m_pixelCanvas);

        // Don't fire if clicking below the ground
        if (clickPos.y < GROUND_Y) {
            return;
        }

        Entity nearestBattery = findNearestBattery(world, clickPos);

        if (nearestBattery != 0) {
            spawnPlayerMissile(world, nearestBattery, clickPos);
        }
    }
}

Entity MouseInputSystem::findNearestBattery(World& world, const Vec2& clickPos) {
    Entity nearest = 0;
    float minDistance = std::numeric_limits<float>::max();

    for (Entity entity : world.query<MissileBattery, Transform>()) {
        auto& battery = world.getComponent<MissileBattery>(entity);

        // Skip destroyed batteries or those with no ammo
        if (battery.isDestroyed || battery.missiles <= 0) {
            continue;
        }

        const auto& transform = world.getComponent<Transform>(entity);
        float distance = std::abs(transform.position.x - clickPos.x);

        if (distance < minDistance) {
            minDistance = distance;
            nearest = entity;
        }
    }

    return nearest;
}

void MouseInputSystem::spawnPlayerMissile(World& world, Entity battery, const Vec2& target) {
    auto& batteryComp = world.getComponent<MissileBattery>(battery);
    const auto& batteryTransform = world.getComponent<Transform>(battery);

    // Decrease ammo
    batteryComp.missiles--;

    // Create missile entity
    Entity missile = world.createEntity();

    // Start position is at the top of the battery
    Vec2 origin(batteryTransform.position.x, batteryTransform.position.y + BATTERY_HEIGHT / 2.0f);

    world.addComponent(missile, Transform{origin.x, origin.y});
    world.addComponent(missile, Sprite{Vec2{MISSILE_HEAD_SIZE, MISSILE_HEAD_SIZE}, PLAYER_MISSILE_COLOR});
    world.addComponent(missile, PlayerMissile{origin, target, PLAYER_MISSILE_SPEED});
}

} // namespace delphinis
