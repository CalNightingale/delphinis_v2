#include "InputSystem.h"
#include <delphinis/ecs/World.h>
#include "components/PaddleInput.h"
#include <delphinis/components/Velocity.h>
#include <delphinis/components/Transform.h>
#include <delphinis/components/Sprite.h>
#include "constants/PongConstants.h"

namespace delphinis {

InputSystem::InputSystem(GLFWwindow* window, float viewHeight)
    : m_window(window), m_viewHeight(viewHeight) {
}

void InputSystem::update(World& world, float deltaTime) {
    for (Entity entity : world.query<PaddleInput, Velocity, Transform, Sprite>()) {
        const auto& input = world.getComponent<PaddleInput>(entity);
        auto& velocity = world.getComponent<Velocity>(entity);
        const auto& transform = world.getComponent<Transform>(entity);
        const auto& sprite = world.getComponent<Sprite>(entity);

        velocity.velocity.y = 0.0f;

        // Calculate paddle bounds
        float paddleHalfHeight = sprite.size.y / 2.0f;
        float currentY = transform.position.y;

        // Calculate screen boundaries keeping buffer amount visible
        // Top: paddle's bottom edge should stay at least BUFFER below top of screen
        float maxY = m_viewHeight / 2.0f - PADDLE_VISIBILITY_BUFFER + paddleHalfHeight;
        // Bottom: paddle's top edge should stay at least BUFFER above bottom of screen
        float minY = -m_viewHeight / 2.0f + PADDLE_VISIBILITY_BUFFER - paddleHalfHeight;

        if (glfwGetKey(m_window, input.upKey) == GLFW_PRESS) {
            // Only allow upward movement if paddle won't exceed visibility buffer
            if (currentY < maxY) {
                velocity.velocity.y = input.speed;
            }
        }
        if (glfwGetKey(m_window, input.downKey) == GLFW_PRESS) {
            // Only allow downward movement if paddle won't exceed visibility buffer
            if (currentY > minY) {
                velocity.velocity.y = -input.speed;
            }
        }
    }
}

} // namespace delphinis
