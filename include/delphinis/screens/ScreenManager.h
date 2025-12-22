#pragma once

#include "Screen.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace delphinis {

class ScreenManager {
public:
    explicit ScreenManager(GLFWwindow* window);
    ~ScreenManager();

    // Stack operations
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void changeScreen(std::unique_ptr<Screen> screen); // Convenience: pop + push

    // Main loop integration
    void update(float deltaTime);
    void render();
    void handleInput();

    // Query
    bool isEmpty() const { return m_screenStack.empty(); }
    Screen* getCurrentScreen() const;
    size_t getStackSize() const { return m_screenStack.size(); }

private:
    GLFWwindow* m_window;
    std::vector<std::unique_ptr<Screen>> m_screenStack;
};

} // namespace delphinis
