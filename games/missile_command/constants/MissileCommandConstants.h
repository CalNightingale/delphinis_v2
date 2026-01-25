#pragma once

#include <delphinis/math/Vec2.h>
#include <delphinis/components/Sprite.h>

namespace delphinis {

// ============================================================================
// GAME PHYSICS & SIMULATION
// ============================================================================

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr float MAX_DELTA_TIME = 0.25f;

// ============================================================================
// RETRO RESOLUTION
// ============================================================================

constexpr int CANVAS_WIDTH = 256;
constexpr int CANVAS_HEIGHT = 192;

// ============================================================================
// WORLD LAYOUT (in pixels, origin at center)
// ============================================================================

constexpr float GROUND_Y = -90.0f;
constexpr float GROUND_HEIGHT = 4.0f;
constexpr float CITY_Y = -80.0f;
constexpr float BATTERY_Y = -80.0f;

// City positions (6 cities, spaced ~35px apart)
constexpr float CITY_SPACING = 35.0f;
constexpr float CITY_START_X = -105.0f;  // First city X position
constexpr float CITY_WIDTH = 16.0f;
constexpr float CITY_HEIGHT = 10.0f;

// Battery positions (left, center, right)
constexpr float BATTERY_LEFT_X = -110.0f;
constexpr float BATTERY_CENTER_X = 0.0f;
constexpr float BATTERY_RIGHT_X = 110.0f;
constexpr float BATTERY_WIDTH = 12.0f;
constexpr float BATTERY_HEIGHT = 8.0f;

// ============================================================================
// MISSILES
// ============================================================================

constexpr float PLAYER_MISSILE_SPEED = 150.0f;
constexpr float ENEMY_MISSILE_BASE_SPEED = 20.0f;
constexpr float ENEMY_MISSILE_SPEED_INCREMENT = 5.0f;  // Per wave

constexpr float MISSILE_HEAD_SIZE = 3.0f;
constexpr float MISSILE_TRAIL_WIDTH = 1.0f;

// ============================================================================
// EXPLOSIONS
// ============================================================================

constexpr float EXPLOSION_MAX_RADIUS = 15.0f;
constexpr float EXPLOSION_EXPAND_SPEED = 50.0f;
constexpr float EXPLOSION_HOLD_TIME = 0.3f;
constexpr float EXPLOSION_CONTRACT_SPEED = 40.0f;

// ============================================================================
// WAVES
// ============================================================================

constexpr int BASE_ENEMIES_PER_WAVE = 10;
constexpr int ENEMIES_INCREMENT_PER_WAVE = 5;
constexpr float BASE_SPAWN_INTERVAL = 1.5f;
constexpr float SPAWN_INTERVAL_DECREMENT = 0.1f;
constexpr float MIN_SPAWN_INTERVAL = 0.3f;

constexpr int STARTING_AMMO_PER_BATTERY = 10;

// ============================================================================
// SCORING
// ============================================================================

constexpr int POINTS_PER_ENEMY = 25;
constexpr int POINTS_PER_CITY_BONUS = 100;
constexpr int POINTS_PER_AMMO_BONUS = 5;

// ============================================================================
// COLORS
// ============================================================================

inline const Vec3 BACKGROUND_COLOR{0.0f, 0.0f, 0.1f};
inline const Vec3 GROUND_COLOR{0.6f, 0.4f, 0.2f};
inline const Vec3 CITY_COLOR{0.2f, 0.6f, 1.0f};
inline const Vec3 CITY_DESTROYED_COLOR{0.2f, 0.2f, 0.3f};
inline const Vec3 BATTERY_COLOR{0.0f, 0.8f, 0.0f};
inline const Vec3 BATTERY_DESTROYED_COLOR{0.2f, 0.3f, 0.2f};
inline const Vec3 PLAYER_MISSILE_COLOR{0.0f, 1.0f, 1.0f};
inline const Vec3 PLAYER_TRAIL_COLOR{0.0f, 0.5f, 0.5f};
inline const Vec3 ENEMY_MISSILE_COLOR{1.0f, 0.0f, 0.0f};
inline const Vec3 ENEMY_TRAIL_COLOR{1.0f, 0.3f, 0.3f};
inline const Vec3 EXPLOSION_COLOR{1.0f, 1.0f, 0.3f};
inline const Vec3 CROSSHAIR_COLOR{0.0f, 1.0f, 0.0f};
inline const Vec3 HUD_COLOR{1.0f, 1.0f, 1.0f};

// ============================================================================
// HUD
// ============================================================================

constexpr float HUD_Y = 85.0f;
constexpr float SCORE_X = -120.0f;
constexpr float WAVE_X = 100.0f;
constexpr float HUD_TEXT_SIZE = 5.0f;  // Larger for pixel font on 256x192 canvas

// ============================================================================
// MATH
// ============================================================================

constexpr float PI = 3.14159265359f;

} // namespace delphinis
