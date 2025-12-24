#pragma once

#include <delphinis/ecs/System.h>
#include <GLFW/glfw3.h>

namespace delphinis {

class InputSystem : public System {
public:
    InputSystem(GLFWwindow* window, float viewHeight);

    void update(World& world, float deltaTime) override;

private:
    GLFWwindow* m_window;
    float m_viewHeight;
};

} // namespace delphinis
