#pragma once

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include "Shader.h"
#include "Camera.h"
#include "delphinis/math/Vec2.h"
#include "delphinis/components/Sprite.h"

namespace delphinis {

class TexturedQuadRenderer {
public:
    TexturedQuadRenderer();
    ~TexturedQuadRenderer();

    void init();
    void drawQuad(
        const Vec2& position,
        const Vec2& size,
        const Vec2& uvMin,
        const Vec2& uvMax,
        const Vec3& color,
        GLuint textureId,
        const Camera& camera
    );

private:
    GLuint m_vao;
    GLuint m_vbo;
    Shader m_shader;
    bool m_initialized;
};

} // namespace delphinis
