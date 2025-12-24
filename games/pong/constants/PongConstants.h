#pragma once

#include "../../../include/delphinis/math/Vec2.h"
#include "../../../include/delphinis/components/Sprite.h"
#include <GLFW/glfw3.h>

namespace delphinis {

// ============================================================================
// GAME PHYSICS & SIMULATION
// ============================================================================

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;  // Fixed update timestep (60 FPS)
constexpr float MAX_DELTA_TIME = 0.25f;         // Cap for deltaTime to prevent spiral of death

// ============================================================================
// BALL CONSTANTS
// ============================================================================

constexpr float BALL_SIZE = 0.4f;                          // Ball width and height
constexpr float BALL_INITIAL_VELOCITY_X = 5.0f;            // Initial horizontal velocity
constexpr float BALL_INITIAL_VELOCITY_Y = 3.0f;            // Initial vertical velocity
constexpr float BALL_SPEED = 8.0f;                         // Constant ball speed maintained by BallSystem
inline const Vec3 BALL_COLOR{1.0f, 1.0f, 0.3f};            // Ball color (yellowish)
inline const Vec2 BALL_RESET_POSITION{0.0f, 0.0f};         // Ball reset position after scoring
constexpr float BALL_RESET_VELOCITY_X = 5.0f;              // Ball X velocity after reset
constexpr int BALL_RANDOM_Y_RANGE = 200;                   // Range for random() % calculation
constexpr int BALL_RANDOM_Y_OFFSET = 100;                  // Offset for random Y calculation
constexpr float BALL_RANDOM_Y_DIVISOR = 50.0f;             // Divisor for random Y (produces -2.0 to 2.0)
constexpr float BALL_MIN_SPEED_CHECK = 0.1f;               // Minimum speed threshold for normalization

// ============================================================================
// PADDLE CONSTANTS
// ============================================================================

constexpr float PADDLE_WIDTH = 0.4f;                       // Paddle width
constexpr float PADDLE_HEIGHT = 2.0f;                      // Paddle height
constexpr float PADDLE_OFFSET_FROM_EDGE = 1.0f;            // Distance from screen edge
constexpr float PADDLE_MAX_REFLECTION_ANGLE = 60.0f;       // Maximum bounce angle in degrees
constexpr float PADDLE_VISIBILITY_BUFFER = 0.5f;           // Keeps this many units of the paddle visible on screen

// Player Paddle
constexpr float PLAYER_PADDLE_SPEED = 10.0f;               // Player paddle movement speed
inline const Vec3 PLAYER_PADDLE_COLOR{0.3f, 0.7f, 1.0f};   // Player paddle color (blue)
constexpr int PLAYER_KEY_UP = GLFW_KEY_W;                  // Player up key
constexpr int PLAYER_KEY_DOWN = GLFW_KEY_S;                // Player down key

// AI Paddle
constexpr float AI_PADDLE_SPEED = 6.0f;                    // AI paddle movement speed
inline const Vec3 AI_PADDLE_COLOR{1.0f, 0.3f, 0.3f};       // AI paddle color (red)
constexpr float AI_DEAD_ZONE = 0.5f;                       // Dead zone where AI doesn't move
constexpr float AI_DIRECTION_MULTIPLIER = 1.0f;            // Speed multiplier when moving

// ============================================================================
// WALL CONSTANTS
// ============================================================================

constexpr float WALL_THICKNESS = 0.5f;                     // Top and bottom wall thickness
inline const Vec3 WALL_COLOR{0.4f, 0.4f, 0.4f};            // Wall color (gray)

// ============================================================================
// SCORE DISPLAY CONSTANTS
// ============================================================================

constexpr float SCORE_VERTICAL_OFFSET = 2.0f;              // Distance from top of screen
constexpr float SCORE_HORIZONTAL_DIVISOR = 4.0f;           // Divider for horizontal positioning (viewWidth/4)
constexpr float SCORE_TEXT_SIZE = 2.0f;                    // Score text size
inline const char* SCORE_INITIAL_VALUE = "0";              // Initial score text

// ============================================================================
// END SCREEN CONSTANTS
// ============================================================================

constexpr float WIN_TEXT_Y_POSITION = 2.0f;                // Y position of win message
constexpr float WIN_TEXT_SIZE = 3.0f;                      // Win message text size
constexpr float INSTRUCTIONS_Y_POSITION = -2.0f;           // Y position of instructions
constexpr float INSTRUCTIONS_TEXT_SIZE = 1.0f;             // Instructions text size
inline const Vec3 INSTRUCTIONS_COLOR{0.7f, 0.7f, 0.7f};    // Instructions text color (gray)
inline const char* WIN_MESSAGE_PLAYER = "YOU WIN";         // Player win message
inline const char* WIN_MESSAGE_AI = "RED WINS";            // AI win message
inline const char* RESTART_MESSAGE = "Press ENTER to play again";  // Restart instructions
constexpr int RESTART_KEY = GLFW_KEY_ENTER;                // Key to restart game

// ============================================================================
// RENDERING CONSTANTS
// ============================================================================

inline const Vec3 BACKGROUND_COLOR{0.1f, 0.1f, 0.15f};     // Background color (dark blue)
constexpr float BACKGROUND_ALPHA = 1.0f;                   // Background alpha

// ============================================================================
// MATH CONSTANTS
// ============================================================================

constexpr float PI = 3.14159265359f;                       // Pi constant for angle calculations

} // namespace delphinis
