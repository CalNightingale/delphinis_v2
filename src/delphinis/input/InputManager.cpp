#include "delphinis/input/InputManager.h"
#include "delphinis/renderer/PixelCanvas.h"
#include <cstring>

namespace delphinis {

InputManager::InputManager(GLFWwindow* window)
    : m_window(window) {
    std::memset(m_mouseButtonStates, 0, sizeof(m_mouseButtonStates));
    std::memset(m_prevMouseButtonStates, 0, sizeof(m_prevMouseButtonStates));
    std::memset(m_keyStates, 0, sizeof(m_keyStates));
    std::memset(m_prevKeyStates, 0, sizeof(m_prevKeyStates));
}

Vec2 InputManager::getMousePosition(const PixelCanvas& canvas) const {
    double mouseX, mouseY;
    glfwGetCursorPos(m_window, &mouseX, &mouseY);

    // Use window size (not framebuffer size) to match cursor position coordinates
    // On retina displays, framebuffer is 2x window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_window, &windowWidth, &windowHeight);

    // Calculate the viewport (same logic as PixelCanvas::display)
    float targetAspect = static_cast<float>(canvas.getWidth()) / static_cast<float>(canvas.getHeight());
    float windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    int viewportX, viewportY, viewportW, viewportH;

    if (windowAspect > targetAspect) {
        viewportH = windowHeight;
        viewportW = static_cast<int>(windowHeight * targetAspect);
        viewportX = (windowWidth - viewportW) / 2;
        viewportY = 0;
    } else {
        viewportW = windowWidth;
        viewportH = static_cast<int>(windowWidth / targetAspect);
        viewportX = 0;
        viewportY = (windowHeight - viewportH) / 2;
    }

    // Convert mouse position to canvas coordinates
    // Mouse Y is flipped (0 at top in GLFW, but we want 0 at bottom for OpenGL)
    float canvasX = static_cast<float>(mouseX - viewportX) / viewportW * canvas.getWidth();
    float canvasY = static_cast<float>(windowHeight - mouseY - viewportY) / viewportH * canvas.getHeight();

    // Convert from pixel coords (0,0 at bottom-left) to world coords (0,0 at center)
    float worldX = canvasX - canvas.getWidth() / 2.0f;
    float worldY = canvasY - canvas.getHeight() / 2.0f;

    return Vec2(worldX, worldY);
}

bool InputManager::isMouseButtonPressed(int button) const {
    if (button < 0 || button > 2) return false;
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

bool InputManager::isMouseButtonJustPressed(int button) {
    if (button < 0 || button > 2) return false;
    bool current = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    return current && !m_prevMouseButtonStates[button];
}

bool InputManager::isMouseButtonJustReleased(int button) {
    if (button < 0 || button > 2) return false;
    bool current = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    return !current && m_prevMouseButtonStates[button];
}

bool InputManager::isKeyPressed(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool InputManager::isKeyJustPressed(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    bool current = glfwGetKey(m_window, key) == GLFW_PRESS;
    return current && !m_prevKeyStates[key];
}

void InputManager::update() {
    // Update mouse button previous states
    for (int i = 0; i < 3; i++) {
        m_prevMouseButtonStates[i] = glfwGetMouseButton(m_window, i) == GLFW_PRESS;
    }

    // Update key previous states (only commonly used keys to avoid overhead)
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_Z; key++) {
        m_prevKeyStates[key] = glfwGetKey(m_window, key) == GLFW_PRESS;
    }
    // Also track some special keys
    m_prevKeyStates[GLFW_KEY_ESCAPE] = glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    m_prevKeyStates[GLFW_KEY_ENTER] = glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS;
}

} // namespace delphinis
