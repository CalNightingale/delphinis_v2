#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
    #include <emscripten.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

// Screen Stack
#include "delphinis/screens/ScreenManager.h"
#include "PongGameScreen.h"
#include "StartMenuScreen.h"

// Systems
#include "systems/InputSystem.h"
#include "systems/AISystem.h"
#include "delphinis/systems/MovementSystem.h"
#include "delphinis/systems/CollisionSystem.h"
#include "systems/PaddleCollisionSystem.h"
#include "systems/BallSystem.h"
#include "delphinis/systems/RenderSystem.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/renderer/Camera.h"

using namespace delphinis;

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Pong - Delphinis Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

#ifndef __EMSCRIPTEN__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
#endif

    int windowWidth = 800;
    int windowHeight = 600;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    std::cout << "Pong - Delphinis Engine v0.1.0" << std::endl;
    std::cout << "Controls: W/S for left paddle, SPACE to start" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    // Game view dimensions
    float viewWidth = 20.0f;
    float viewHeight = 15.0f;

    // Create Camera
    Camera camera(viewWidth, viewHeight);

    // Create Systems (owned by main, shared by screens)
    RenderSystem renderSystem(viewWidth, viewHeight);
    MovementSystem movementSystem;
    InputSystem inputSystem(window, viewHeight);
    AISystem aiSystem;
    CollisionSystem collisionSystem;
    PaddleCollisionSystem paddleCollisionSystem;
    BallSystem ballSystem(viewWidth);

#ifdef __EMSCRIPTEN__
    TextRenderingSystem textRenderSystem(camera, "/games/pong/assets/bit5x3.ttf");
#else
    TextRenderingSystem textRenderSystem(camera, "../games/pong/assets/bit5x3.ttf");
#endif

    // Create screen manager
    ScreenManager screenManager(window);

    // Create game screen (pass system references)
    auto gameScreen = std::make_unique<PongGameScreen>(
        renderSystem, textRenderSystem, movementSystem,
        collisionSystem, paddleCollisionSystem, inputSystem, aiSystem, ballSystem,
        viewWidth, viewHeight
    );

    // Set screen manager on game screen (so it can push EndScreen)
    gameScreen->setScreenManager(&screenManager);

    // Create and push start menu (pass game screen ownership)
    screenManager.pushScreen(
        std::make_unique<StartMenuScreen>(
            textRenderSystem, screenManager, std::move(gameScreen)
        )
    );

    // Frame time tracking
    static double lastTime = glfwGetTime();

    // Simplified main loop
    auto mainLoop = [&]() {
        // Calculate delta time
        double currentTime = glfwGetTime();
        float frameTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Fallback input handling
        processInput(window);

        // Update and render through screen manager
        screenManager.handleInput();
        screenManager.update(frameTime);
        screenManager.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    };

#ifdef __EMSCRIPTEN__
    // For Emscripten, use the browser's request animation frame
    emscripten_set_main_loop_arg([](void* arg) {
        auto& loop = *static_cast<decltype(mainLoop)*>(arg);
        loop();
    }, &mainLoop, 0, 1);
#else
    // For native builds, use traditional game loop
    while (!glfwWindowShouldClose(window)) {
        mainLoop();
    }
#endif

    glfwTerminate();
    return 0;
}
