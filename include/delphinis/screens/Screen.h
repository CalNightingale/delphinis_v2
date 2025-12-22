#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "delphinis/ecs/World.h"
#include <memory>

namespace delphinis {

class Screen {
public:
    Screen();
    virtual ~Screen() = default;

    // Lifecycle hooks
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    // Core update/render methods
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

    // Input handling - return true to consume input
    virtual bool handleInput(GLFWwindow* window) { return false; }

    // Screen behavior policies
    virtual bool blocksUpdate() const { return true; }
    virtual bool blocksRender() const { return false; }

    // World access
    World& getWorld() { return *m_world; }
    const World& getWorld() const { return *m_world; }

protected:
    std::unique_ptr<World> m_world;
};

} // namespace delphinis
