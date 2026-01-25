#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
    #include <emscripten.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

#include "delphinis/screens/ScreenManager.h"
#include "delphinis/renderer/PixelCanvas.h"
#include "delphinis/input/InputManager.h"
#include "delphinis/renderer/Camera.h"
#include "delphinis/audio/AudioManager.h"

#include "MissileCommandGameScreen.h"
#include "StartMenuScreen.h"
#include "constants/MissileCommandConstants.h"

using namespace delphinis;

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    // Don't set viewport here - PixelCanvas manages its own viewport
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

    // Start with a larger window for better visibility
    GLFWwindow* window = glfwCreateWindow(768, 576, "Missile Command - Delphinis Engine", nullptr, nullptr);
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

    std::cout << "Missile Command - Delphinis Engine v0.1.0" << std::endl;
    std::cout << "Click to fire missiles at incoming threats!" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    // Initialize PixelCanvas for retro resolution
    PixelCanvas pixelCanvas;
    pixelCanvas.init(CANVAS_WIDTH, CANVAS_HEIGHT);

    // Create Camera for the retro resolution
    Camera camera(static_cast<float>(CANVAS_WIDTH), static_cast<float>(CANVAS_HEIGHT));

    // Create InputManager
    InputManager inputManager(window);

    // Initialize audio
    AudioManager audioManager;
    if (!audioManager.initialize()) {
        std::cerr << "Warning: Audio initialization failed, continuing without sound" << std::endl;
    }

    // Create screen manager
    ScreenManager screenManager(window);

    // Create game screen
    auto gameScreen = std::make_unique<MissileCommandGameScreen>(
        camera, pixelCanvas, inputManager, audioManager
    );
    gameScreen->setScreenManager(&screenManager);

    // Create and push start menu
    screenManager.pushScreen(
        std::make_unique<StartMenuScreen>(camera, screenManager, std::move(gameScreen))
    );

    // Frame time tracking
    static double lastTime = glfwGetTime();

    // Mute key tracking
    static bool mKeyWasPressed = false;

    // Main loop
    auto mainLoop = [&]() {
        double currentTime = glfwGetTime();
        float frameTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        processInput(window);

        // Mute toggle
        bool mKeyIsPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
        if (mKeyIsPressed && !mKeyWasPressed) {
            audioManager.toggleMute();
            std::cout << "Audio " << (audioManager.isMuted() ? "muted" : "unmuted") << std::endl;
        }
        mKeyWasPressed = mKeyIsPressed;

        // Get window size for rendering
        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        // Clear the actual window first (for letterboxing)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render to PixelCanvas
        pixelCanvas.beginRender();

        // Let the screen manager handle update and render
        screenManager.handleInput();
        screenManager.update(frameTime);
        screenManager.render();

        pixelCanvas.endRender();

        // Display the canvas to the window
        pixelCanvas.display(windowWidth, windowHeight);

        // Update input manager for next frame
        inputManager.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg([](void* arg) {
        auto& loop = *static_cast<decltype(mainLoop)*>(arg);
        loop();
    }, &mainLoop, 0, 1);
#else
    while (!glfwWindowShouldClose(window)) {
        mainLoop();
    }
#endif

    audioManager.shutdown();
    glfwTerminate();
    return 0;
}
