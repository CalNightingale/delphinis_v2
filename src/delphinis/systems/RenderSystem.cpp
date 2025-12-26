#include "delphinis/systems/RenderSystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Sprite.h"

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

namespace delphinis {

RenderSystem::RenderSystem(float viewWidth, float viewHeight)
    : m_camera(viewWidth, viewHeight) {
    m_quadRenderer.init();
}

void RenderSystem::update(World& world, float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (Entity entity : world.query<Transform, Sprite>()) {
        const auto& transform = world.getComponent<Transform>(entity);
        const auto& sprite = world.getComponent<Sprite>(entity);

        m_quadRenderer.drawQuad(
            transform.position,
            sprite.size,
            sprite.color,
            m_camera
        );
    }
}

void RenderSystem::setViewport(float width, float height) {
    m_camera.setViewport(width, height);
}

} // namespace delphinis
