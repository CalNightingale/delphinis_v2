#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
    #include <emscripten.h>
    #include <emscripten/html5.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// ECS
#include "delphinis/ecs/World.h"

// Components
#include "delphinis/components/Transform.h"
#include "delphinis/components/Velocity.h"
#include "delphinis/components/BoxCollider.h"
#include "delphinis/components/Sprite.h"
#include "components/PaddleInput.h"
#include "components/AIController.h"
#include "components/Ball.h"

// Systems
#include "systems/InputSystem.h"
#include "systems/AISystem.h"
#include "delphinis/systems/MovementSystem.h"
#include "delphinis/systems/CollisionSystem.h"
#include "systems/BallSystem.h"
#include "delphinis/systems/RenderSystem.h"

using namespace delphinis;

#ifdef __EMSCRIPTEN__
// Global flag to control game loop execution in web builds
// Prevents game from starting until user clicks on canvas
static bool gameStarted = false;

// JavaScript-callable function to start the game
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void startGame() {
        gameStarted = true;
    }
}
#endif

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
    std::cout << "Controls: W/S for left paddle" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Create ECS World
    World world;

    // Game view dimensions
    float viewWidth = 20.0f;
    float viewHeight = 15.0f;

    // Create Systems
    RenderSystem renderSystem(viewWidth, viewHeight);
    MovementSystem movementSystem;
    InputSystem inputSystem(window);
    AISystem aiSystem;
    CollisionSystem collisionSystem;
    BallSystem ballSystem;

    // Create Ball Entity
    Entity ball = world.createEntity();
    world.addComponent(ball, Transform{0.0f, 0.0f});
    world.addComponent(ball, Velocity{5.0f, 3.0f});
    world.addComponent(ball, BoxCollider{0.4f, 0.4f});
    world.addComponent(ball, Sprite{Vec2{0.4f, 0.4f}, Vec3{1.0f, 1.0f, 0.3f}});
    world.addComponent(ball, Ball{8.0f});

    // Create Left Paddle (Player)
    Entity leftPaddle = world.createEntity();
    world.addComponent(leftPaddle, Transform{-viewWidth/2 + 1.0f, 0.0f});
    world.addComponent(leftPaddle, Velocity{0.0f, 0.0f});
    world.addComponent(leftPaddle, BoxCollider{0.4f, 2.0f});
    world.addComponent(leftPaddle, Sprite{Vec2{0.4f, 2.0f}, Vec3{0.3f, 0.7f, 1.0f}});
    world.addComponent(leftPaddle, PaddleInput{GLFW_KEY_W, GLFW_KEY_S, 10.0f});

    // Create Right Paddle (AI)
    Entity rightPaddle = world.createEntity();
    world.addComponent(rightPaddle, Transform{viewWidth/2 - 1.0f, 0.0f});
    world.addComponent(rightPaddle, Velocity{0.0f, 0.0f});
    world.addComponent(rightPaddle, BoxCollider{0.4f, 2.0f});
    world.addComponent(rightPaddle, Sprite{Vec2{0.4f, 2.0f}, Vec3{1.0f, 0.3f, 0.3f}});
    world.addComponent(rightPaddle, AIController{8.0f, ball});

    // Create Top Wall
    Entity topWall = world.createEntity();
    world.addComponent(topWall, Transform{0.0f, viewHeight/2});
    world.addComponent(topWall, BoxCollider{viewWidth, 0.5f});
    world.addComponent(topWall, Sprite{Vec2{viewWidth, 0.5f}, Vec3{0.4f, 0.4f, 0.4f}});

    // Create Bottom Wall
    Entity bottomWall = world.createEntity();
    world.addComponent(bottomWall, Transform{0.0f, -viewHeight/2});
    world.addComponent(bottomWall, BoxCollider{viewWidth, 0.5f});
    world.addComponent(bottomWall, Sprite{Vec2{viewWidth, 0.5f}, Vec3{0.4f, 0.4f, 0.4f}});

    // Fixed timestep game loop
    const float FIXED_DT = 1.0f / 60.0f;
    static float accumulator = 0.0f;
    static double lastTime = glfwGetTime();

    // FPS counter
    static int frameCount = 0;
    static double fpsLastTime = glfwGetTime();
    static int currentFPS = 0;

    // Game loop function
    auto mainLoop = [&]() {
#ifdef __EMSCRIPTEN__
        // In web builds, pause game logic until user clicks to start
        // This prevents the ball from moving before the player is ready
        if (!gameStarted) {
            // Still render the initial state and handle window events
            renderSystem.update(world, 0.0f);
            glfwSwapBuffers(window);
            glfwPollEvents();
            return;
        }
#endif

        double currentTime = glfwGetTime();
        float frameTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Cap frame time to prevent spiral of death
        if (frameTime > 0.25f) {
            frameTime = 0.25f;
        }

        accumulator += frameTime;

        // Fixed update loop
        while (accumulator >= FIXED_DT) {
            processInput(window);

            // Update systems in order
            inputSystem.update(world, FIXED_DT);
            aiSystem.update(world, FIXED_DT);
            movementSystem.update(world, FIXED_DT);
            collisionSystem.update(world, FIXED_DT);
            ballSystem.update(world, FIXED_DT);

            accumulator -= FIXED_DT;
        }

        // Render
        renderSystem.update(world, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update FPS counter
        frameCount++;
        double fpsCurrentTime = glfwGetTime();
        if (fpsCurrentTime - fpsLastTime >= 0.5) {
            currentFPS = static_cast<int>(frameCount / (fpsCurrentTime - fpsLastTime));
            frameCount = 0;
            fpsLastTime = fpsCurrentTime;

            std::string title = "Pong - Delphinis Engine (FPS: " + std::to_string(currentFPS) + ")";
            glfwSetWindowTitle(window, title.c_str());
        }
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
