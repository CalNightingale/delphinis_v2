#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/systems/RenderSystem.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/systems/MovementSystem.h"
#include "delphinis/systems/CollisionSystem.h"
#include "systems/InputSystem.h"
#include "systems/AISystem.h"
#include "systems/BallSystem.h"
#include "delphinis/ecs/Entity.h"

namespace delphinis {

class PongGameScreen : public Screen {
public:
    PongGameScreen(
        RenderSystem& renderSystem,
        TextRenderingSystem& textRenderingSystem,
        MovementSystem& movementSystem,
        CollisionSystem& collisionSystem,
        InputSystem& inputSystem,
        AISystem& aiSystem,
        BallSystem& ballSystem,
        float viewWidth,
        float viewHeight
    );

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;

    // Game fills screen completely
    bool blocksUpdate() const override { return true; }
    bool blocksRender() const override { return true; }

private:
    // System references
    RenderSystem& m_renderSystem;
    TextRenderingSystem& m_textRenderSystem;
    MovementSystem& m_movementSystem;
    CollisionSystem& m_collisionSystem;
    InputSystem& m_inputSystem;
    AISystem& m_aiSystem;
    BallSystem& m_ballSystem;

    // Game dimensions
    float m_viewWidth;
    float m_viewHeight;

    // Fixed timestep accumulator
    float m_accumulator;

    // Track entities for score updates
    Entity m_leftScoreText;
    Entity m_rightScoreText;
};

} // namespace delphinis
