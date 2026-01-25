#pragma once

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "delphinis/math/Vec2.h"

namespace delphinis {

// Forward declaration
class PixelCanvas;

class InputManager {
public:
    InputManager(GLFWwindow* window);

    // Get mouse position in canvas/world coordinates
    Vec2 getMousePosition(const PixelCanvas& canvas) const;

    // Mouse button state
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonJustPressed(int button);
    bool isMouseButtonJustReleased(int button);

    // Keyboard state
    bool isKeyPressed(int key) const;
    bool isKeyJustPressed(int key);

    // Call once per frame to update previous states
    void update();

private:
    GLFWwindow* m_window;
    bool m_mouseButtonStates[3];
    bool m_prevMouseButtonStates[3];
    bool m_keyStates[GLFW_KEY_LAST + 1];
    bool m_prevKeyStates[GLFW_KEY_LAST + 1];
};

} // namespace delphinis
