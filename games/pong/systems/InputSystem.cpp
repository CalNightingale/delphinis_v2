#include "InputSystem.h"
#include "../../../include/delphinis/ecs/World.h"
#include "../components/PaddleInput.h"
#include "../../../include/delphinis/components/Velocity.h"

namespace delphinis {

InputSystem::InputSystem(GLFWwindow* window) : m_window(window) {
}

void InputSystem::update(World& world, float deltaTime) {
    for (Entity entity : world.getEntities()) {
        if (world.hasComponents<PaddleInput, Velocity>(entity)) {
            const auto& input = world.getComponent<PaddleInput>(entity);
            auto& velocity = world.getComponent<Velocity>(entity);

            velocity.velocity.y = 0.0f;

            if (glfwGetKey(m_window, input.upKey) == GLFW_PRESS) {
                velocity.velocity.y = input.speed;
            }
            if (glfwGetKey(m_window, input.downKey) == GLFW_PRESS) {
                velocity.velocity.y = -input.speed;
            }
        }
    }
}

} // namespace delphinis
