#include "PongGameScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"
#include "delphinis/components/BoxCollider.h"
#include "delphinis/components/Sprite.h"
#include "delphinis/components/Text.h"
#include "components/PaddleInput.h"
#include "components/AIController.h"
#include "components/Ball.h"
#include <GLFW/glfw3.h>

namespace delphinis {

PongGameScreen::PongGameScreen(
    RenderSystem& renderSystem,
    TextRenderingSystem& textRenderSystem,
    MovementSystem& movementSystem,
    CollisionSystem& collisionSystem,
    InputSystem& inputSystem,
    AISystem& aiSystem,
    BallSystem& ballSystem,
    float viewWidth,
    float viewHeight
)
    : m_renderSystem(renderSystem)
    , m_textRenderSystem(textRenderSystem)
    , m_movementSystem(movementSystem)
    , m_collisionSystem(collisionSystem)
    , m_inputSystem(inputSystem)
    , m_aiSystem(aiSystem)
    , m_ballSystem(ballSystem)
    , m_viewWidth(viewWidth)
    , m_viewHeight(viewHeight)
    , m_accumulator(0.0f)
{
}

void PongGameScreen::onEnter() {
    // Create Ball Entity
    Entity ball = getWorld().createEntity();
    getWorld().addComponent(ball, Transform{0.0f, 0.0f});
    getWorld().addComponent(ball, Velocity{5.0f, 3.0f});
    getWorld().addComponent(ball, BoxCollider{0.4f, 0.4f});
    getWorld().addComponent(ball, Sprite{Vec2{0.4f, 0.4f}, Vec3{1.0f, 1.0f, 0.3f}});
    getWorld().addComponent(ball, Ball{8.0f});

    // Create Left Paddle (Player)
    Entity leftPaddle = getWorld().createEntity();
    getWorld().addComponent(leftPaddle, Transform{-m_viewWidth/2 + 1.0f, 0.0f});
    getWorld().addComponent(leftPaddle, Velocity{0.0f, 0.0f});
    getWorld().addComponent(leftPaddle, BoxCollider{0.4f, 2.0f});
    getWorld().addComponent(leftPaddle, Sprite{Vec2{0.4f, 2.0f}, Vec3{0.3f, 0.7f, 1.0f}});
    getWorld().addComponent(leftPaddle, PaddleInput{GLFW_KEY_W, GLFW_KEY_S, 10.0f});

    // Create Right Paddle (AI)
    Entity rightPaddle = getWorld().createEntity();
    getWorld().addComponent(rightPaddle, Transform{m_viewWidth/2 - 1.0f, 0.0f});
    getWorld().addComponent(rightPaddle, Velocity{0.0f, 0.0f});
    getWorld().addComponent(rightPaddle, BoxCollider{0.4f, 2.0f});
    getWorld().addComponent(rightPaddle, Sprite{Vec2{0.4f, 2.0f}, Vec3{1.0f, 0.3f, 0.3f}});
    getWorld().addComponent(rightPaddle, AIController{8.0f, ball});

    // Create Top Wall
    Entity topWall = getWorld().createEntity();
    getWorld().addComponent(topWall, Transform{0.0f, m_viewHeight/2});
    getWorld().addComponent(topWall, BoxCollider{m_viewWidth, 0.5f});
    getWorld().addComponent(topWall, Sprite{Vec2{m_viewWidth, 0.5f}, Vec3{0.4f, 0.4f, 0.4f}});

    // Create Bottom Wall
    Entity bottomWall = getWorld().createEntity();
    getWorld().addComponent(bottomWall, Transform{0.0f, -m_viewHeight/2});
    getWorld().addComponent(bottomWall, BoxCollider{m_viewWidth, 0.5f});
    getWorld().addComponent(bottomWall, Sprite{Vec2{m_viewWidth, 0.5f}, Vec3{0.4f, 0.4f, 0.4f}});

    // Create Score Display Entities
    m_leftScoreText = getWorld().createEntity();
    getWorld().addComponent(m_leftScoreText, Transform{-m_viewWidth/4, m_viewHeight/2 - 2.0f});
    getWorld().addComponent(m_leftScoreText, Text{"0", Vec3{0.3f, 0.7f, 1.0f}, 2.0f, TextAlign::Center});

    m_rightScoreText = getWorld().createEntity();
    getWorld().addComponent(m_rightScoreText, Transform{m_viewWidth/4, m_viewHeight/2 - 2.0f});
    getWorld().addComponent(m_rightScoreText, Text{"0", Vec3{1.0f, 0.3f, 0.3f}, 2.0f, TextAlign::Center});
}

void PongGameScreen::update(float deltaTime) {
    const float FIXED_DT = 1.0f / 60.0f;

    // Cap delta time to prevent spiral of death
    if (deltaTime > 0.25f) {
        deltaTime = 0.25f;
    }

    m_accumulator += deltaTime;

    // Fixed update loop
    while (m_accumulator >= FIXED_DT) {
        // Update systems in order
        m_inputSystem.update(getWorld(), FIXED_DT);
        m_aiSystem.update(getWorld(), FIXED_DT);
        m_movementSystem.update(getWorld(), FIXED_DT);
        m_collisionSystem.update(getWorld(), FIXED_DT);
        m_ballSystem.update(getWorld(), FIXED_DT);

        // Update score displays
        auto& leftText = getWorld().getComponent<Text>(m_leftScoreText);
        auto& rightText = getWorld().getComponent<Text>(m_rightScoreText);
        leftText.content = std::to_string(m_ballSystem.getLeftScore());
        rightText.content = std::to_string(m_ballSystem.getRightScore());

        m_accumulator -= FIXED_DT;
    }
}

void PongGameScreen::render() {
    // Clear screen
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render sprites and text
    m_renderSystem.update(getWorld(), 0.0f);
    m_textRenderSystem.update(getWorld(), 0.0f);
}

} // namespace delphinis
