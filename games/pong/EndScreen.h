#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/screens/ScreenManager.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/systems/RenderSystem.h"
#include "delphinis/systems/MovementSystem.h"
#include "delphinis/systems/CollisionSystem.h"
#include "systems/InputSystem.h"
#include "systems/AISystem.h"
#include "systems/BallSystem.h"
#include <memory>

namespace delphinis {

class EndScreen : public Screen {
public:
    EndScreen(
        TextRenderingSystem& textRenderSystem,
        ScreenManager& screenManager,
        RenderSystem& renderSystem,
        MovementSystem& movementSystem,
        CollisionSystem& collisionSystem,
        InputSystem& inputSystem,
        AISystem& aiSystem,
        BallSystem& ballSystem,
        float viewWidth,
        float viewHeight,
        bool playerWon
    );

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    bool handleInput(GLFWwindow* window) override;

    // End screen fills screen completely
    bool blocksUpdate() const override { return true; }
    bool blocksRender() const override { return true; }

private:
    TextRenderingSystem& m_textRenderSystem;
    ScreenManager& m_screenManager;
    RenderSystem& m_renderSystem;
    MovementSystem& m_movementSystem;
    CollisionSystem& m_collisionSystem;
    InputSystem& m_inputSystem;
    AISystem& m_aiSystem;
    BallSystem& m_ballSystem;
    float m_viewWidth;
    float m_viewHeight;
    bool m_playerWon;
};

} // namespace delphinis
