#include "StartMenuScreen.h"
#include "constants/MissileCommandConstants.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"

namespace delphinis {

StartMenuScreen::StartMenuScreen(
    Camera& camera,
    ScreenManager& screenManager,
    std::unique_ptr<MissileCommandGameScreen> gameScreen
)
    : m_camera(camera)
    , m_screenManager(screenManager)
    , m_gameScreen(std::move(gameScreen))
{
}

void StartMenuScreen::onEnter() {
    m_quadRenderer.init();

#ifdef __EMSCRIPTEN__
    m_textRenderSystem = std::make_unique<TextRenderingSystem>(m_camera, "/games/missile_command/assets/bit5x3.ttf");
#else
    m_textRenderSystem = std::make_unique<TextRenderingSystem>(m_camera, "../games/missile_command/assets/bit5x3.ttf");
#endif

    // Title - larger sizes for pixel font on 256x192 canvas
    Entity title = getWorld().createEntity();
    getWorld().addComponent(title, Transform{0.0f, 50.0f});
    getWorld().addComponent(title, Text{"MISSILE COMMAND", Vec3(1.0f, 0.0f, 0.0f), 10.0f, TextAlign::Center});

    // Subtitle
    Entity subtitle = getWorld().createEntity();
    getWorld().addComponent(subtitle, Transform{0.0f, 20.0f});
    getWorld().addComponent(subtitle, Text{"Defend your cities!", Vec3(1.0f, 1.0f, 1.0f), 6.0f, TextAlign::Center});

    // Start prompt
    Entity startPrompt = getWorld().createEntity();
    getWorld().addComponent(startPrompt, Transform{0.0f, -30.0f});
    getWorld().addComponent(startPrompt, Text{"Click to start", Vec3(0.0f, 1.0f, 0.0f), 6.0f, TextAlign::Center});
}

void StartMenuScreen::update(float deltaTime) {
    // Nothing to update
}

void StartMenuScreen::render() {
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_textRenderSystem->update(getWorld(), 0.0f);
}

bool StartMenuScreen::handleInput(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        m_screenManager.queueChangeScreen(std::move(m_gameScreen));
        return true;
    }
    return false;
}

} // namespace delphinis
