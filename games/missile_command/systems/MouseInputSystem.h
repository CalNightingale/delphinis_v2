#pragma once

#include "delphinis/ecs/World.h"
#include "delphinis/input/InputManager.h"
#include "delphinis/renderer/PixelCanvas.h"

namespace delphinis {

class MouseInputSystem {
public:
    MouseInputSystem(InputManager& inputManager, PixelCanvas& pixelCanvas);

    void update(World& world, float deltaTime);

private:
    InputManager& m_inputManager;
    PixelCanvas& m_pixelCanvas;

    Entity findNearestBattery(World& world, const Vec2& clickPos);
    void spawnPlayerMissile(World& world, Entity battery, const Vec2& target);
};

} // namespace delphinis
