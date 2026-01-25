#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/screens/ScreenManager.h"
#include "delphinis/renderer/Camera.h"
#include "delphinis/renderer/QuadRenderer.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "MissileCommandGameScreen.h"
#include <memory>

namespace delphinis {

class StartMenuScreen : public Screen {
public:
    StartMenuScreen(
        Camera& camera,
        ScreenManager& screenManager,
        std::unique_ptr<MissileCommandGameScreen> gameScreen
    );

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    bool handleInput(GLFWwindow* window) override;

private:
    Camera& m_camera;
    ScreenManager& m_screenManager;
    std::unique_ptr<MissileCommandGameScreen> m_gameScreen;

    QuadRenderer m_quadRenderer;
    std::unique_ptr<TextRenderingSystem> m_textRenderSystem;
};

} // namespace delphinis
