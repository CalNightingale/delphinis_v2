#include "delphinis/screens/ScreenManager.h"

namespace delphinis {

ScreenManager::ScreenManager(GLFWwindow* window)
    : m_window(window) {
}

ScreenManager::~ScreenManager() {
    // Clean up all screens in reverse order
    while (!m_screenStack.empty()) {
        popScreen();
    }
}

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen) {
    // Pause current screen if exists
    if (!m_screenStack.empty()) {
        m_screenStack.back()->onPause();
    }

    // Add and enter new screen
    m_screenStack.push_back(std::move(screen));
    m_screenStack.back()->onEnter();
}

void ScreenManager::popScreen() {
    if (m_screenStack.empty()) {
        return;
    }

    // Exit current screen
    m_screenStack.back()->onExit();

    // Remove from stack
    m_screenStack.pop_back();

    // Resume previous screen if exists
    if (!m_screenStack.empty()) {
        m_screenStack.back()->onResume();
    }
}

void ScreenManager::changeScreen(std::unique_ptr<Screen> screen) {
    popScreen();
    pushScreen(std::move(screen));
}

void ScreenManager::update(float deltaTime) {
    if (m_screenStack.empty()) {
        return;
    }

    // Find which screens should update (from top to bottom until we hit a blocker)
    std::vector<Screen*> screensToUpdate;
    for (auto it = m_screenStack.rbegin(); it != m_screenStack.rend(); ++it) {
        screensToUpdate.push_back(it->get());
        if ((*it)->blocksUpdate()) {
            break; // This screen blocks lower screens from updating
        }
    }

    // Update in reverse order (bottom to top)
    for (auto it = screensToUpdate.rbegin(); it != screensToUpdate.rend(); ++it) {
        (*it)->update(deltaTime);
    }

    // Process any screen transitions that were queued during update
    processPendingActions();
}

void ScreenManager::render() {
    if (m_screenStack.empty()) {
        return;
    }

    // Find which screens should render (from top to bottom until we hit a blocker)
    std::vector<Screen*> screensToRender;
    for (auto it = m_screenStack.rbegin(); it != m_screenStack.rend(); ++it) {
        screensToRender.push_back(it->get());
        if ((*it)->blocksRender()) {
            break; // This screen blocks lower screens from rendering
        }
    }

    // Render in reverse order (bottom to top = back to front)
    for (auto it = screensToRender.rbegin(); it != screensToRender.rend(); ++it) {
        (*it)->render();
    }
}

void ScreenManager::handleInput() {
    // Input goes from top to bottom - first screen to consume input stops propagation
    for (auto it = m_screenStack.rbegin(); it != m_screenStack.rend(); ++it) {
        if ((*it)->handleInput(m_window)) {
            break; // Input consumed
        }
    }

    // Process any screen transitions that were queued during input handling
    processPendingActions();
}

void ScreenManager::queuePushScreen(std::unique_ptr<Screen> screen) {
    m_pendingActions.push({ActionType::Push, std::move(screen)});
}

void ScreenManager::queuePopScreen() {
    m_pendingActions.push({ActionType::Pop, nullptr});
}

void ScreenManager::queueChangeScreen(std::unique_ptr<Screen> screen) {
    m_pendingActions.push({ActionType::Change, std::move(screen)});
}

void ScreenManager::processPendingActions() {
    while (!m_pendingActions.empty()) {
        auto& action = m_pendingActions.front();

        switch (action.type) {
            case ActionType::Push:
                pushScreen(std::move(action.screen));
                break;

            case ActionType::Pop:
                popScreen();
                break;

            case ActionType::Change:
                changeScreen(std::move(action.screen));
                break;
        }

        m_pendingActions.pop();
    }
}

Screen* ScreenManager::getCurrentScreen() const {
    return m_screenStack.empty() ? nullptr : m_screenStack.back().get();
}

} // namespace delphinis
