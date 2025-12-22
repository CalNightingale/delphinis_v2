#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/screens/ScreenManager.h"
#include "delphinis/systems/TextRenderingSystem.h"

class SecondScreen : public delphinis::Screen {
public:
    SecondScreen(delphinis::TextRenderingSystem& textRenderSystem, delphinis::ScreenManager& screenManager);

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;
    bool handleInput(GLFWwindow* window) override;

    // Block lower screens from updating, but allow them to render
    bool blocksUpdate() const override { return true; }
    bool blocksRender() const override { return false; }

private:
    delphinis::TextRenderingSystem& m_textRenderSystem;
    delphinis::ScreenManager& m_screenManager;
};
