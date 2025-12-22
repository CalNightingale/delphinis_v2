#include "HelloScreen.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"

using namespace delphinis;

HelloScreen::HelloScreen(TextRenderingSystem& textRenderSystem)
    : m_textRenderSystem(textRenderSystem) {
}

void HelloScreen::onEnter() {
    // Create centered text entity in this screen's world
    Entity helloText = getWorld().createEntity();
    getWorld().addComponent(helloText, Transform{{0.0f, 0.0f}});  // Center of screen
    getWorld().addComponent(helloText, Text{"hello world!", Vec3{1.0f, 1.0f, 0.3f}, 2.5f, TextAlign::Center});
}

void HelloScreen::update(float deltaTime) {
    // No update logic for hello screen
    (void)deltaTime; // Suppress unused parameter warning
}

void HelloScreen::render() {
    // Clear screen and render this screen's world
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_textRenderSystem.update(getWorld(), 0.0f);
}
