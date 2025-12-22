#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "delphinis/screens/ScreenManager.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/renderer/Camera.h"
#include "HelloScreen.h"
#include "SecondScreen.h"

using namespace delphinis;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Delphinis Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set initial viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    std::cout << "Delphinis Engine v0.1.0" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Press ESC to pop screen (exit when all screens are gone)" << std::endl;

    // Create Camera and TextRenderingSystem (owned by main.cpp, shared by screens)
    Camera camera(20.0f, 15.0f);

#ifdef __EMSCRIPTEN__
    TextRenderingSystem textRenderSystem(camera, "/games/hello/assets/bit5x3.ttf");
#else
    TextRenderingSystem textRenderSystem(camera, "../games/hello/assets/bit5x3.ttf");
#endif

    // Create screen manager
    ScreenManager screenManager(window);

    // Push initial screen
    screenManager.pushScreen(std::make_unique<HelloScreen>(textRenderSystem));

    // Immediately push second screen (automatic on startup)
    screenManager.pushScreen(std::make_unique<SecondScreen>(textRenderSystem, screenManager));

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input (fallback if no screens handle it)
        processInput(window);

        // Update and render screens
        screenManager.handleInput();
        screenManager.update(0.0f);
        screenManager.render();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
