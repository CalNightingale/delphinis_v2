#include "PongGameScreen.h"
#include "EndScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"
#include "delphinis/components/BoxCollider.h"
#include "delphinis/components/Sprite.h"
#include "delphinis/components/Text.h"
#include "components/PaddleInput.h"
#include "components/AIController.h"
#include "components/Ball.h"
#include "components/PaddleCollider.h"
#include "components/Wall.h"
#include "constants/PongConstants.h"
#include <GLFW/glfw3.h>

namespace delphinis {

PongGameScreen::PongGameScreen(
    RenderSystem& renderSystem,
    TextRenderingSystem& textRenderSystem,
    MovementSystem& movementSystem,
    CollisionSystem& collisionSystem,
    PaddleCollisionSystem& paddleCollisionSystem,
    WallCollisionSystem& wallCollisionSystem,
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
    , m_paddleCollisionSystem(paddleCollisionSystem)
    , m_wallCollisionSystem(wallCollisionSystem)
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
    getWorld().addComponent(ball, Velocity{BALL_INITIAL_VELOCITY_X, BALL_INITIAL_VELOCITY_Y});
    getWorld().addComponent(ball, BoxCollider{BALL_SIZE, BALL_SIZE});
    getWorld().addComponent(ball, Sprite{Vec2{BALL_SIZE, BALL_SIZE}, BALL_COLOR});
    getWorld().addComponent(ball, Ball{BALL_SPEED});

    // Create Left Paddle (Player)
    Entity leftPaddle = getWorld().createEntity();
    getWorld().addComponent(leftPaddle, Transform{-m_viewWidth/2 + PADDLE_OFFSET_FROM_EDGE, 0.0f});
    getWorld().addComponent(leftPaddle, Velocity{0.0f, 0.0f});
    getWorld().addComponent(leftPaddle, Sprite{Vec2{PADDLE_WIDTH, PADDLE_HEIGHT}, PLAYER_PADDLE_COLOR});
    getWorld().addComponent(leftPaddle, PaddleInput{PLAYER_KEY_UP, PLAYER_KEY_DOWN, PLAYER_PADDLE_SPEED});
    getWorld().addComponent(leftPaddle, PaddleCollider{PADDLE_MAX_REFLECTION_ANGLE});

    // Create Right Paddle (AI)
    Entity rightPaddle = getWorld().createEntity();
    getWorld().addComponent(rightPaddle, Transform{m_viewWidth/2 - PADDLE_OFFSET_FROM_EDGE, 0.0f});
    getWorld().addComponent(rightPaddle, Velocity{0.0f, 0.0f});
    getWorld().addComponent(rightPaddle, Sprite{Vec2{PADDLE_WIDTH, PADDLE_HEIGHT}, AI_PADDLE_COLOR});
    getWorld().addComponent(rightPaddle, AIController{AI_PADDLE_SPEED, ball});
    getWorld().addComponent(rightPaddle, PaddleCollider{PADDLE_MAX_REFLECTION_ANGLE});

    // Create Top Wall
    Entity topWall = getWorld().createEntity();
    getWorld().addComponent(topWall, Transform{0.0f, m_viewHeight/2});
    getWorld().addComponent(topWall, BoxCollider{m_viewWidth, WALL_THICKNESS});
    getWorld().addComponent(topWall, Sprite{Vec2{m_viewWidth, WALL_THICKNESS}, WALL_COLOR});
    getWorld().addComponent(topWall, Wall{WallType::Top});

    // Create Bottom Wall
    Entity bottomWall = getWorld().createEntity();
    getWorld().addComponent(bottomWall, Transform{0.0f, -m_viewHeight/2});
    getWorld().addComponent(bottomWall, BoxCollider{m_viewWidth, WALL_THICKNESS});
    getWorld().addComponent(bottomWall, Sprite{Vec2{m_viewWidth, WALL_THICKNESS}, WALL_COLOR});
    getWorld().addComponent(bottomWall, Wall{WallType::Bottom});

    // Create Score Display Entities
    m_leftScoreText = getWorld().createEntity();
    getWorld().addComponent(m_leftScoreText, Transform{-m_viewWidth/SCORE_HORIZONTAL_DIVISOR, m_viewHeight/2 - SCORE_VERTICAL_OFFSET});
    getWorld().addComponent(m_leftScoreText, Text{SCORE_INITIAL_VALUE, PLAYER_PADDLE_COLOR, SCORE_TEXT_SIZE, TextAlign::Center});

    m_rightScoreText = getWorld().createEntity();
    getWorld().addComponent(m_rightScoreText, Transform{m_viewWidth/SCORE_HORIZONTAL_DIVISOR, m_viewHeight/2 - SCORE_VERTICAL_OFFSET});
    getWorld().addComponent(m_rightScoreText, Text{SCORE_INITIAL_VALUE, AI_PADDLE_COLOR, SCORE_TEXT_SIZE, TextAlign::Center});
}

void PongGameScreen::update(float deltaTime) {
    // Cap delta time to prevent spiral of death
    if (deltaTime > MAX_DELTA_TIME) {
        deltaTime = MAX_DELTA_TIME;
    }

    m_accumulator += deltaTime;

    // Fixed update loop
    while (m_accumulator >= FIXED_TIMESTEP) {
        // Update systems in order
        m_inputSystem.update(getWorld(), FIXED_TIMESTEP);
        m_aiSystem.update(getWorld(), FIXED_TIMESTEP);
        m_movementSystem.update(getWorld(), FIXED_TIMESTEP);
        m_paddleCollisionSystem.update(getWorld(), FIXED_TIMESTEP);
        m_wallCollisionSystem.update(getWorld(), FIXED_TIMESTEP);
        m_collisionSystem.update(getWorld(), FIXED_TIMESTEP);
        m_ballSystem.update(getWorld(), FIXED_TIMESTEP);

        // Update score displays
        auto& leftText = getWorld().getComponent<Text>(m_leftScoreText);
        auto& rightText = getWorld().getComponent<Text>(m_rightScoreText);
        leftText.content = std::to_string(m_ballSystem.getLeftScore());
        rightText.content = std::to_string(m_ballSystem.getRightScore());

        m_accumulator -= FIXED_TIMESTEP;
    }

    // Check for game over
    if (m_ballSystem.isGameOver() && m_screenManager) {
        bool playerWon = m_ballSystem.didPlayerWin();

        // Reset scores for next game
        m_ballSystem.resetScores();

        // Create and push the end screen
        auto endScreen = std::make_unique<EndScreen>(
            m_textRenderSystem, *m_screenManager,
            m_renderSystem, m_movementSystem,
            m_collisionSystem, m_paddleCollisionSystem, m_wallCollisionSystem,
            m_inputSystem, m_aiSystem, m_ballSystem,
            m_viewWidth, m_viewHeight, playerWon
        );

        m_screenManager->queuePushScreen(std::move(endScreen));
    }
}

void PongGameScreen::render() {
    // Clear screen
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render sprites and text
    m_renderSystem.update(getWorld(), 0.0f);
    m_textRenderSystem.update(getWorld(), 0.0f);
}

} // namespace delphinis
