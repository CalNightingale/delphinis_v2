#pragma once

#include <GLFW/glfw3.h>

namespace delphinis {

struct PaddleInput {
    int upKey{GLFW_KEY_W};
    int downKey{GLFW_KEY_S};
    float speed{8.0f};

    PaddleInput() = default;
    PaddleInput(int up, int down, float spd) : upKey(up), downKey(down), speed(spd) {}
};

} // namespace delphinis
