#pragma once

#include "delphinis/ecs/System.h"
#include "delphinis/renderer/Camera.h"
#include "delphinis/renderer/QuadRenderer.h"

namespace delphinis {

class RenderSystem : public System {
public:
    RenderSystem(float viewWidth, float viewHeight);

    void update(World& world, float deltaTime) override;
    void setViewport(float width, float height);

private:
    Camera m_camera;
    QuadRenderer m_quadRenderer;
};

} // namespace delphinis
