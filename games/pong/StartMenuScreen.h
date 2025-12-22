#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/screens/ScreenManager.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include <memory>

namespace delphinis {

class StartMenuScreen : public Screen {
public:
    StartMenuScreen(
        TextRenderingSystem& textRenderSystem,
        ScreenManager& screenManager,
        std::unique_ptr<Screen> gameScreen
    );

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    bool handleInput(GLFWwindow* window) override;

    // Menu fills screen completely
    bool blocksUpdate() const override { return true; }
    bool blocksRender() const override { return true; }

private:
    TextRenderingSystem& m_textRenderSystem;
    ScreenManager& m_screenManager;
    std::unique_ptr<Screen> m_gameScreen;
};

} // namespace delphinis
