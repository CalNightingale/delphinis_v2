#include "SecondScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"

using namespace delphinis;

SecondScreen::SecondScreen(TextRenderingSystem& textRenderSystem, ScreenManager& screenManager)
    : m_textRenderSystem(textRenderSystem)
    , m_screenManager(screenManager) {
}

void SecondScreen::onEnter() {
    // Create text entity above center in this screen's world
    Entity secondText = getWorld().createEntity();
    getWorld().addComponent(secondText, Transform{{0.0f, 3.0f}});  // Above center
    getWorld().addComponent(secondText, Text{"hello from another screen", Vec3{0.3f, 1.0f, 0.3f}, 2.0f, TextAlign::Center});
}

void SecondScreen::update(float deltaTime) {
    // No update logic for second screen
    (void)deltaTime; // Suppress unused parameter warning
}

void SecondScreen::render() {
    // Render on top of the previous screen (don't clear)
    m_textRenderSystem.update(getWorld(), 0.0f);
}

bool SecondScreen::handleInput(GLFWwindow* window) {
    // ESC key pops this screen
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_screenManager.popScreen();
        return true; // Consume input
    }
    return false; // Don't consume input
}
