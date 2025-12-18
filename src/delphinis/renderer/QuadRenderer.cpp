#include "delphinis/renderer/QuadRenderer.h"
#include "delphinis/math/Mat4.h"

namespace delphinis {

#ifdef __EMSCRIPTEN__
static const char* vertexShaderSource = R"(#version 300 es
precision mediump float;
layout (location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform mat4 uModel;

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
}
)";

static const char* fragmentShaderSource = R"(#version 300 es
precision mediump float;
out vec4 FragColor;

uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor, 1.0);
}
)";
#else
static const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform mat4 uModel;

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
}
)";

static const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;

uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor, 1.0);
}
)";
#endif

QuadRenderer::QuadRenderer()
    : m_vao(0), m_vbo(0), m_initialized(false) {
}

QuadRenderer::~QuadRenderer() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void QuadRenderer::init() {
    if (m_initialized) return;

    // Unit quad vertices (centered at origin)
    float vertices[] = {
        -0.5f, -0.5f,  // Bottom-left
         0.5f, -0.5f,  // Bottom-right
         0.5f,  0.5f,  // Top-right
        -0.5f,  0.5f,  // Top-left
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shader
    m_shader.loadFromSource(vertexShaderSource, fragmentShaderSource);

    m_initialized = true;
}

void QuadRenderer::drawQuad(const Vec2& position, const Vec2& size, const Vec3& color, const Camera& camera) {
    if (!m_initialized) return;

    m_shader.use();

    // Set uniforms
    m_shader.setMat4("uProjection", camera.getProjectionMatrix());

    // Create model matrix (translate and scale)
    Mat4 model = Mat4::translate(position.x, position.y) * Mat4::scale(size.x, size.y);
    m_shader.setMat4("uModel", model);

    m_shader.setVec3("uColor", color);

    // Draw quad
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

} // namespace delphinis
