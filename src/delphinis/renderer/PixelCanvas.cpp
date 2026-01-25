#include "delphinis/renderer/PixelCanvas.h"
#include <iostream>

namespace delphinis {

#ifdef __EMSCRIPTEN__
static const char* canvasVertexShader = R"(#version 300 es
precision mediump float;
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

static const char* canvasFragmentShader = R"(#version 300 es
precision mediump float;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, TexCoord);
}
)";
#else
static const char* canvasVertexShader = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

static const char* canvasFragmentShader = R"(#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, TexCoord);
}
)";
#endif

PixelCanvas::PixelCanvas()
    : m_fbo(0), m_texture(0), m_vao(0), m_vbo(0)
    , m_width(0), m_height(0), m_initialized(false) {
}

PixelCanvas::~PixelCanvas() {
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }
    if (m_texture != 0) {
        glDeleteTextures(1, &m_texture);
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void PixelCanvas::init(int width, int height) {
    if (m_initialized) return;

    m_width = width;
    m_height = height;

    // Create framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create texture for framebuffer
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Use nearest neighbor filtering for blocky pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "PixelCanvas: Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create fullscreen quad for displaying the texture
    float vertices[] = {
        // positions   // texcoords
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shader
    m_shader.loadFromSource(canvasVertexShader, canvasFragmentShader);

    m_initialized = true;
}

void PixelCanvas::beginRender() {
    if (!m_initialized) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void PixelCanvas::endRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PixelCanvas::display(int windowWidth, int windowHeight) {
    if (!m_initialized) return;

    // Calculate viewport to maintain aspect ratio with letterboxing
    float targetAspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    float windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    int viewportX, viewportY, viewportW, viewportH;

    if (windowAspect > targetAspect) {
        // Window is wider than target - pillarbox
        viewportH = windowHeight;
        viewportW = static_cast<int>(windowHeight * targetAspect);
        viewportX = (windowWidth - viewportW) / 2;
        viewportY = 0;
    } else {
        // Window is taller than target - letterbox
        viewportW = windowWidth;
        viewportH = static_cast<int>(windowWidth / targetAspect);
        viewportX = 0;
        viewportY = (windowHeight - viewportH) / 2;
    }

    glViewport(viewportX, viewportY, viewportW, viewportH);

    m_shader.use();
    m_shader.setInt("uTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

} // namespace delphinis
