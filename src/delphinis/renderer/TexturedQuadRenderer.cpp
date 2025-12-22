#include "delphinis/renderer/TexturedQuadRenderer.h"
#include "delphinis/math/Mat4.h"

namespace delphinis {

#ifdef __EMSCRIPTEN__
static const char* vertexShaderSource = R"(#version 300 es
precision mediump float;
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uModel;

out vec2 vTexCoord;

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";

static const char* fragmentShaderSource = R"(#version 300 es
precision mediump float;

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uColor;

void main() {
    float alpha = texture(uTexture, vTexCoord).r;
    FragColor = vec4(uColor, alpha);
}
)";
#else
static const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uModel;

out vec2 vTexCoord;

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
}
)";

static const char* fragmentShaderSource = R"(#version 330 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uColor;

void main() {
    float alpha = texture(uTexture, vTexCoord).r;
    FragColor = vec4(uColor, alpha);
}
)";
#endif

TexturedQuadRenderer::TexturedQuadRenderer()
    : m_vao(0), m_vbo(0), m_initialized(false) {
}

TexturedQuadRenderer::~TexturedQuadRenderer() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void TexturedQuadRenderer::init() {
    if (m_initialized) return;

    // Unit quad vertices with texture coordinates (centered at origin)
    // Format: posX, posY, texU, texV
    float vertices[] = {
        -0.5f, -0.5f,  0.0f, 1.0f,  // Bottom-left
         0.5f, -0.5f,  1.0f, 1.0f,  // Bottom-right
         0.5f,  0.5f,  1.0f, 0.0f,  // Top-right
        -0.5f,  0.5f,  0.0f, 0.0f,  // Top-left
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shader
    m_shader.loadFromSource(vertexShaderSource, fragmentShaderSource);

    m_initialized = true;
}

void TexturedQuadRenderer::drawQuad(
    const Vec2& position,
    const Vec2& size,
    const Vec2& uvMin,
    const Vec2& uvMax,
    const Vec3& color,
    GLuint textureId,
    const Camera& camera
) {
    if (!m_initialized) return;

    m_shader.use();

    // Set uniforms
    m_shader.setMat4("uProjection", camera.getProjectionMatrix());

    // Create model matrix (translate and scale)
    Mat4 model = Mat4::translate(position.x, position.y) * Mat4::scale(size.x, size.y);
    m_shader.setMat4("uModel", model);

    m_shader.setVec3("uColor", color);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    m_shader.setInt("uTexture", 0);

    // Update texture coordinates in VBO if they differ from (0,0) to (1,1)
    if (uvMin.x != 0.0f || uvMin.y != 0.0f || uvMax.x != 1.0f || uvMax.y != 1.0f) {
        float vertices[] = {
            -0.5f, -0.5f,  uvMin.x, uvMax.y,  // Bottom-left
             0.5f, -0.5f,  uvMax.x, uvMax.y,  // Bottom-right
             0.5f,  0.5f,  uvMax.x, uvMin.y,  // Top-right
            -0.5f,  0.5f,  uvMin.x, uvMin.y,  // Top-left
        };
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Draw quad
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

} // namespace delphinis
