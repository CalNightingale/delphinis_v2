#pragma once

#include "Screen.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <queue>

namespace delphinis {

class ScreenManager {
public:
    explicit ScreenManager(GLFWwindow* window);
    ~ScreenManager();

    // Immediate stack operations (use with caution - prefer queued versions)
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void changeScreen(std::unique_ptr<Screen> screen); // Convenience: pop + push

    // Deferred/safe stack operations (recommended for use from within screens)
    void queuePushScreen(std::unique_ptr<Screen> screen);
    void queuePopScreen();
    void queueChangeScreen(std::unique_ptr<Screen> screen);

    // Main loop integration
    void update(float deltaTime);
    void render();
    void handleInput();

    // Query
    bool isEmpty() const { return m_screenStack.empty(); }
    Screen* getCurrentScreen() const;
    size_t getStackSize() const { return m_screenStack.size(); }

private:
    enum class ActionType {
        Push,
        Pop,
        Change
    };

    struct PendingAction {
        ActionType type;
        std::unique_ptr<Screen> screen; // Only used for Push and Change
    };

    GLFWwindow* m_window;
    std::vector<std::unique_ptr<Screen>> m_screenStack;
    std::queue<PendingAction> m_pendingActions;

    void processPendingActions();
};

} // namespace delphinis
