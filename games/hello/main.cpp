#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "delphinis/ecs/World.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"
#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/renderer/Camera.h"

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
    std::cout << "Press ESC to exit" << std::endl;

    // Create ECS World
    World world;

    // Create Camera
    Camera camera(20.0f, 15.0f);

    // Create TextRenderingSystem
    TextRenderingSystem textRenderSystem(camera);

    // Create centered text entity
    Entity helloText = world.createEntity();
    world.addComponent(helloText, Transform{{0.0f, 0.0f}});  // Center of screen
    world.addComponent(helloText, Text{"hello world!", Vec3{1.0f, 1.0f, 0.3f}, 1.0f, TextAlign::Center});

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update text rendering system
        textRenderSystem.update(world, 0.0f);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
