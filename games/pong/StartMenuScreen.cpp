#include "StartMenuScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"
#include <GLFW/glfw3.h>

namespace delphinis {

StartMenuScreen::StartMenuScreen(
    TextRenderingSystem& textRenderSystem,
    ScreenManager& screenManager,
    std::unique_ptr<Screen> gameScreen
)
    : m_textRenderSystem(textRenderSystem)
    , m_screenManager(screenManager)
    , m_gameScreen(std::move(gameScreen))
{
}

void StartMenuScreen::onEnter() {
    // Create title text
    Entity titleText = getWorld().createEntity();
    getWorld().addComponent(titleText, Transform{0.0f, 2.0f});
    getWorld().addComponent(titleText, Text{"PONG", Vec3{1.0f, 1.0f, 1.0f}, 4.0f, TextAlign::Center});

    // Create instructions text
    Entity instructionsText = getWorld().createEntity();
    getWorld().addComponent(instructionsText, Transform{0.0f, -2.0f});
    getWorld().addComponent(instructionsText, Text{"Press SPACE to start", Vec3{0.7f, 0.7f, 0.7f}, 1.0f, TextAlign::Center});
}

void StartMenuScreen::update(float deltaTime) {
    // No update logic for static menu
    (void)deltaTime; // Suppress unused parameter warning
}

void StartMenuScreen::render() {
    // Clear screen with dark blue background
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render menu text
    m_textRenderSystem.update(getWorld(), 0.0f);
}

bool StartMenuScreen::handleInput(GLFWwindow* window) {
    // SPACE key starts the game (only if we haven't already started)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && m_gameScreen) {
        // Queue the game screen to be pushed (safe - happens after this method returns)
        // The game screen will block the menu from updating/rendering
        m_screenManager.queuePushScreen(std::move(m_gameScreen));
        return true; // Consume input
    }

    return false; // Don't consume input
}

} // namespace delphinis
