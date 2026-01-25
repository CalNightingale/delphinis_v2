#include "MissileCommandGameScreen.h"
#include "constants/MissileCommandConstants.h"
#include "components/City.h"
#include "components/MissileBattery.h"
#include "components/GameState.h"
#include "components/PlayerMissile.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Sprite.h"
#include "delphinis/components/Text.h"
#include <iostream>
#include <cmath>
#include <algorithm>

namespace delphinis {

MissileCommandGameScreen::MissileCommandGameScreen(
    Camera& camera,
    PixelCanvas& pixelCanvas,
    InputManager& inputManager,
    AudioManager& audioManager
)
    : m_camera(camera)
    , m_pixelCanvas(pixelCanvas)
    , m_inputManager(inputManager)
    , m_audioManager(audioManager)
{
}

void MissileCommandGameScreen::onEnter() {
    m_quadRenderer.init();

#ifdef __EMSCRIPTEN__
    m_textRenderSystem = std::make_unique<TextRenderingSystem>(m_camera, "/games/missile_command/assets/bit5x3.ttf");
#else
    m_textRenderSystem = std::make_unique<TextRenderingSystem>(m_camera, "../games/missile_command/assets/bit5x3.ttf");
#endif

    // Create game systems
    m_mouseInputSystem = std::make_unique<MouseInputSystem>(m_inputManager, m_pixelCanvas);
    m_playerMissileSystem = std::make_unique<PlayerMissileSystem>();

    createEntities();
}

void MissileCommandGameScreen::createEntities() {
    // Create GameState entity
    m_gameStateEntity = getWorld().createEntity();
    getWorld().addComponent(m_gameStateEntity, GameState{});

    // City positions - 3 on left side of center battery, 3 on right side
    // Layout: [C0] [C1] [C2] [B0] [C3] [C4] [C5]
    //         -90  -55  -20   0    20   55   90 (roughly)
    float cityPositions[] = { -90.0f, -55.0f, -20.0f, 20.0f, 55.0f, 90.0f };

    // Create 6 cities
    for (int i = 0; i < 6; i++) {
        Entity city = getWorld().createEntity();
        getWorld().addComponent(city, Transform{cityPositions[i], CITY_Y});
        getWorld().addComponent(city, Sprite{Vec2{CITY_WIDTH, CITY_HEIGHT}, CITY_COLOR});
        getWorld().addComponent(city, City{i, false});
    }

    // Create 3 batteries
    float batteryPositions[] = { BATTERY_LEFT_X, BATTERY_CENTER_X, BATTERY_RIGHT_X };
    for (int i = 0; i < 3; i++) {
        Entity battery = getWorld().createEntity();
        getWorld().addComponent(battery, Transform{batteryPositions[i], BATTERY_Y});
        getWorld().addComponent(battery, Sprite{Vec2{BATTERY_WIDTH, BATTERY_HEIGHT}, BATTERY_COLOR});
        getWorld().addComponent(battery, MissileBattery{i, STARTING_AMMO_PER_BATTERY, false});
    }

    // Create HUD text entities
    m_scoreText = getWorld().createEntity();
    getWorld().addComponent(m_scoreText, Transform{SCORE_X, HUD_Y});
    getWorld().addComponent(m_scoreText, Text{"SCORE: 0", HUD_COLOR, HUD_TEXT_SIZE, TextAlign::Left});

    m_waveText = getWorld().createEntity();
    getWorld().addComponent(m_waveText, Transform{WAVE_X, HUD_Y});
    getWorld().addComponent(m_waveText, Text{"WAVE: 1", HUD_COLOR, HUD_TEXT_SIZE, TextAlign::Right});
}

void MissileCommandGameScreen::onExit() {
}

void MissileCommandGameScreen::update(float deltaTime) {
    // Cap delta time
    if (deltaTime > MAX_DELTA_TIME) {
        deltaTime = MAX_DELTA_TIME;
    }

    // Process input every frame (not tied to fixed timestep)
    m_mouseInputSystem->update(getWorld(), deltaTime);

    m_accumulator += deltaTime;

    while (m_accumulator >= FIXED_TIMESTEP) {
        // Update physics/movement at fixed rate
        m_playerMissileSystem->update(getWorld(), FIXED_TIMESTEP);

        m_accumulator -= FIXED_TIMESTEP;
    }

    updateHUD();
}

void MissileCommandGameScreen::updateHUD() {
    auto& gameState = getWorld().getComponent<GameState>(m_gameStateEntity);
    auto& scoreText = getWorld().getComponent<Text>(m_scoreText);
    auto& waveText = getWorld().getComponent<Text>(m_waveText);

    scoreText.content = "SCORE: " + std::to_string(gameState.score);
    waveText.content = "WAVE: " + std::to_string(gameState.wave);
}

void MissileCommandGameScreen::render() {
    // Clear with background color
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderGround();
    renderCities();
    renderBatteries();
    renderPlayerMissiles();
    renderHUD();
}

void MissileCommandGameScreen::renderGround() {
    m_quadRenderer.drawQuad(
        Vec2(0.0f, GROUND_Y),
        Vec2(static_cast<float>(CANVAS_WIDTH), GROUND_HEIGHT),
        GROUND_COLOR,
        m_camera
    );
}

void MissileCommandGameScreen::renderCities() {
    for (Entity entity : getWorld().query<City, Transform, Sprite>()) {
        const auto& city = getWorld().getComponent<City>(entity);
        const auto& transform = getWorld().getComponent<Transform>(entity);
        const auto& sprite = getWorld().getComponent<Sprite>(entity);

        Vec3 color = city.isDestroyed ? CITY_DESTROYED_COLOR : sprite.color;
        m_quadRenderer.drawQuad(transform.position, sprite.size, color, m_camera);
    }
}

void MissileCommandGameScreen::renderBatteries() {
    for (Entity entity : getWorld().query<MissileBattery, Transform, Sprite>()) {
        const auto& battery = getWorld().getComponent<MissileBattery>(entity);
        const auto& transform = getWorld().getComponent<Transform>(entity);
        const auto& sprite = getWorld().getComponent<Sprite>(entity);

        Vec3 color = battery.isDestroyed ? BATTERY_DESTROYED_COLOR : sprite.color;
        m_quadRenderer.drawQuad(transform.position, sprite.size, color, m_camera);
    }
}

void MissileCommandGameScreen::renderPlayerMissiles() {
    for (Entity entity : getWorld().query<PlayerMissile, Transform, Sprite>()) {
        const auto& missile = getWorld().getComponent<PlayerMissile>(entity);
        const auto& transform = getWorld().getComponent<Transform>(entity);
        const auto& sprite = getWorld().getComponent<Sprite>(entity);

        // Draw trail from origin to current position
        Vec2 trailStart = missile.origin;
        Vec2 trailEnd = transform.position;
        Vec2 diff = trailEnd - trailStart;
        float trailLength = diff.length();

        if (trailLength > 0.1f) {
            // Draw a series of small quads along the path for the trail
            int segments = static_cast<int>(trailLength / 2.0f);
            segments = std::max(1, std::min(segments, 50));

            for (int i = 0; i < segments; i++) {
                float t = static_cast<float>(i) / segments;
                Vec2 segPos = trailStart + diff * t;
                m_quadRenderer.drawQuad(segPos, Vec2(2.0f, 1.0f), PLAYER_TRAIL_COLOR, m_camera);
            }
        }

        // Draw missile head
        m_quadRenderer.drawQuad(transform.position, sprite.size, sprite.color, m_camera);
    }
}

void MissileCommandGameScreen::renderHUD() {
    m_textRenderSystem->update(getWorld(), 0.0f);
}

} // namespace delphinis
