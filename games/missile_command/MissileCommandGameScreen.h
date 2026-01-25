#pragma once

#include "delphinis/screens/Screen.h"
#include "delphinis/screens/ScreenManager.h"
#include "delphinis/renderer/Camera.h"
#include "delphinis/renderer/QuadRenderer.h"
#include "delphinis/renderer/PixelCanvas.h"
#include "delphinis/input/InputManager.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/audio/AudioManager.h"
#include "delphinis/ecs/Entity.h"
#include "systems/MouseInputSystem.h"
#include "systems/PlayerMissileSystem.h"

namespace delphinis {

class MissileCommandGameScreen : public Screen {
public:
    MissileCommandGameScreen(
        Camera& camera,
        PixelCanvas& pixelCanvas,
        InputManager& inputManager,
        AudioManager& audioManager
    );

    void onEnter() override;
    void onExit() override;
    void update(float deltaTime) override;
    void render() override;

    void setScreenManager(ScreenManager* screenManager) { m_screenManager = screenManager; }

private:
    Camera& m_camera;
    PixelCanvas& m_pixelCanvas;
    InputManager& m_inputManager;
    AudioManager& m_audioManager;
    ScreenManager* m_screenManager = nullptr;

    QuadRenderer m_quadRenderer;
    std::unique_ptr<TextRenderingSystem> m_textRenderSystem;

    // Game systems
    std::unique_ptr<MouseInputSystem> m_mouseInputSystem;
    std::unique_ptr<PlayerMissileSystem> m_playerMissileSystem;

    float m_accumulator = 0.0f;

    // Entity references
    Entity m_gameStateEntity;
    Entity m_scoreText;
    Entity m_waveText;

    void createEntities();
    void renderGround();
    void renderCities();
    void renderBatteries();
    void renderPlayerMissiles();
    void renderHUD();
    void updateHUD();
};

} // namespace delphinis
