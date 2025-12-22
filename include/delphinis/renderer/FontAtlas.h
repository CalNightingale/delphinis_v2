#pragma once

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include <unordered_map>
#include "delphinis/math/Vec2.h"

namespace delphinis {

struct GlyphInfo {
    Vec2 uvMin;       // Top-left UV coordinate
    Vec2 uvMax;       // Bottom-right UV coordinate
    Vec2 size;        // Glyph size in pixels
    Vec2 offset;      // Offset from baseline
    float advance;    // Horizontal advance to next character
};

class FontAtlas {
public:
    FontAtlas();
    ~FontAtlas();

    void init(const char* fontPath);

    GLuint getTextureId() const { return m_textureId; }
    const GlyphInfo& getGlyph(char c) const;
    float getLineHeight() const { return m_lineHeight; }

private:
    GLuint m_textureId;
    int m_atlasWidth;
    int m_atlasHeight;
    float m_lineHeight;
    std::unordered_map<char, GlyphInfo> m_glyphs;
    bool m_initialized;

    void generateFontAtlas(const char* fontPath);
};

} // namespace delphinis
