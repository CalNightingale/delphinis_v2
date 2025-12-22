#include "delphinis/systems/TextRenderingSystem.h"
#include "delphinis/ecs/World.h"
#include "delphinis/components/Transform.h"
#include "delphinis/components/Text.h"

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

namespace delphinis {

TextRenderingSystem::TextRenderingSystem(Camera& camera)
    : m_camera(camera) {
    m_texturedQuadRenderer.init();
    m_fontAtlas.init();
}

void TextRenderingSystem::update(World& world, float deltaTime) {
    // Enable blending for text transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint fontTexture = m_fontAtlas.getTextureId();

    // Conversion factor: pixels to world units
    // Font is baked at ~20 pixels tall, scale down to reasonable world size
    const float pixelToWorld = 0.02f;

    for (Entity entity : world.getEntities()) {
        if (world.hasComponents<Transform, Text>(entity)) {
            const auto& transform = world.getComponent<Transform>(entity);
            const auto& text = world.getComponent<Text>(entity);

            // Convert from pixel space to world space
            float worldScale = pixelToWorld * text.scale;

            // Calculate total text width for alignment
            float totalWidth = 0.0f;
            for (char c : text.content) {
                if (c < 32 || c > 126) continue;
                const GlyphInfo& glyph = m_fontAtlas.getGlyph(c);
                totalWidth += glyph.advance * worldScale;
            }

            // Calculate starting position based on alignment
            Vec2 cursorPos = transform.position;
            switch (text.align) {
                case TextAlign::Left:
                    // No adjustment needed
                    break;
                case TextAlign::Center:
                    cursorPos.x -= totalWidth * 0.5f;
                    break;
                case TextAlign::Right:
                    cursorPos.x -= totalWidth;
                    break;
            }

            // Iterate through each character
            for (char c : text.content) {
                if (c < 32 || c > 126) continue;  // Skip non-printable

                const GlyphInfo& glyph = m_fontAtlas.getGlyph(c);

                // Calculate quad position and size in world coordinates
                Vec2 glyphSize = Vec2{
                    glyph.size.x * worldScale,
                    glyph.size.y * worldScale
                };

                // Position glyph on baseline
                // cursorPos.y is the baseline
                // glyph.offset.y is offset from baseline to top of glyph (typically negative)
                // In OpenGL (Y-up), we need to flip: -offset.y gives top position above baseline
                // Center of quad: top - height/2
                Vec2 glyphPos = Vec2{
                    cursorPos.x + glyph.offset.x * worldScale + glyphSize.x * 0.5f,
                    cursorPos.y - (glyph.offset.y + glyph.size.y * 0.5f) * worldScale
                };

                // Draw glyph
                m_texturedQuadRenderer.drawQuad(
                    glyphPos,
                    glyphSize,
                    glyph.uvMin,
                    glyph.uvMax,
                    text.color,
                    fontTexture,
                    m_camera
                );

                // Advance cursor
                cursorPos.x += glyph.advance * worldScale;
            }
        }
    }

    glDisable(GL_BLEND);
}

} // namespace delphinis
