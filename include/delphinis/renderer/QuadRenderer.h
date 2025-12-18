#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include "Camera.h"
#include "delphinis/math/Vec2.h"
#include "delphinis/components/Sprite.h"

namespace delphinis {

class QuadRenderer {
public:
    QuadRenderer();
    ~QuadRenderer();

    void init();
    void drawQuad(const Vec2& position, const Vec2& size, const Vec3& color, const Camera& camera);

private:
    GLuint m_vao;
    GLuint m_vbo;
    Shader m_shader;
    bool m_initialized;
};

} // namespace delphinis
