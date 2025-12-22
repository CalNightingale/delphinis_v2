#pragma once

#include "delphinis/ecs/System.h"
#include "delphinis/renderer/Camera.h"
#include "delphinis/renderer/TexturedQuadRenderer.h"
#include "delphinis/renderer/FontAtlas.h"

namespace delphinis {

class TextRenderingSystem : public System {
public:
    TextRenderingSystem(Camera& camera, const char* fontPath);

    void update(World& world, float deltaTime) override;

private:
    Camera& m_camera;
    TexturedQuadRenderer m_texturedQuadRenderer;
    FontAtlas m_fontAtlas;
};

} // namespace delphinis
