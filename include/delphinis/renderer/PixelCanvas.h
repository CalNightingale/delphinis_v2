#pragma once

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include "Shader.h"

namespace delphinis {

class PixelCanvas {
public:
    PixelCanvas();
    ~PixelCanvas();

    void init(int width, int height);
    void beginRender();
    void endRender();
    void display(int windowWidth, int windowHeight);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    GLuint m_fbo;
    GLuint m_texture;
    GLuint m_vao;
    GLuint m_vbo;
    Shader m_shader;
    int m_width;
    int m_height;
    bool m_initialized;
};

} // namespace delphinis
