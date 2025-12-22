#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/systems/TextRenderingSystem.h"

class HelloScreen : public delphinis::Screen {
public:
    explicit HelloScreen(delphinis::TextRenderingSystem& textRenderSystem);

    void onEnter() override;
    void update(float deltaTime) override;
    void render() override;

    // Allow screens below to render
    bool blocksRender() const override { return false; }

private:
    delphinis::TextRenderingSystem& m_textRenderSystem;
};
