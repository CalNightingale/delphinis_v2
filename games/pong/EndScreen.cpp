#include "EndScreen.h"
#include "PongGameScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"
#include "constants/PongConstants.h"
#include <GLFW/glfw3.h>

namespace delphinis {

EndScreen::EndScreen(
    TextRenderingSystem& textRenderSystem,
    ScreenManager& screenManager,
    RenderSystem& renderSystem,
    MovementSystem& movementSystem,
    CollisionSystem& collisionSystem,
    PaddleCollisionSystem& paddleCollisionSystem,
    InputSystem& inputSystem,
    AISystem& aiSystem,
    BallSystem& ballSystem,
    float viewWidth,
    float viewHeight,
    bool playerWon
)
    : m_textRenderSystem(textRenderSystem)
    , m_screenManager(screenManager)
    , m_renderSystem(renderSystem)
    , m_movementSystem(movementSystem)
    , m_collisionSystem(collisionSystem)
    , m_paddleCollisionSystem(paddleCollisionSystem)
    , m_inputSystem(inputSystem)
    , m_aiSystem(aiSystem)
    , m_ballSystem(ballSystem)
    , m_viewWidth(viewWidth)
    , m_viewHeight(viewHeight)
    , m_playerWon(playerWon)
{
}

void EndScreen::onEnter() {
    // Create win message text
    Entity winText = getWorld().createEntity();
    getWorld().addComponent(winText, Transform{0.0f, WIN_TEXT_Y_POSITION});

    if (m_playerWon) {
        getWorld().addComponent(winText, Text{WIN_MESSAGE_PLAYER, PLAYER_PADDLE_COLOR, WIN_TEXT_SIZE, TextAlign::Center});
    } else {
        getWorld().addComponent(winText, Text{WIN_MESSAGE_AI, AI_PADDLE_COLOR, WIN_TEXT_SIZE, TextAlign::Center});
    }

    // Create instructions text
    Entity instructionsText = getWorld().createEntity();
    getWorld().addComponent(instructionsText, Transform{0.0f, INSTRUCTIONS_Y_POSITION});
    getWorld().addComponent(instructionsText, Text{RESTART_MESSAGE, INSTRUCTIONS_COLOR, INSTRUCTIONS_TEXT_SIZE, TextAlign::Center});
}

void EndScreen::update(float deltaTime) {
    // No update logic for static end screen
    (void)deltaTime; // Suppress unused parameter warning
}

void EndScreen::render() {
    // Clear screen with dark background
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render text
    m_textRenderSystem.update(getWorld(), 0.0f);
}

bool EndScreen::handleInput(GLFWwindow* window) {
    // ENTER key starts a new game
    if (glfwGetKey(window, RESTART_KEY) == GLFW_PRESS) {
        // Create a fresh game screen
        auto newGameScreen = std::make_unique<PongGameScreen>(
            m_renderSystem, m_textRenderSystem, m_movementSystem,
            m_collisionSystem, m_paddleCollisionSystem, m_inputSystem, m_aiSystem, m_ballSystem,
            m_viewWidth, m_viewHeight
        );

        // Set screen manager on the new game screen
        newGameScreen->setScreenManager(&m_screenManager);

        // Queue all operations (safe - happens after this method returns)
        m_screenManager.queuePopScreen(); // Pop this end screen
        m_screenManager.queuePopScreen(); // Pop the old game screen
        m_screenManager.queuePushScreen(std::move(newGameScreen)); // Push new game

        return true; // Consume input
    }

    return false; // Don't consume input
}

} // namespace delphinis
