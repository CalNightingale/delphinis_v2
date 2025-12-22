#include "delphinis/renderer/FontAtlas.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

namespace delphinis {

FontAtlas::FontAtlas()
    : m_textureId(0)
    , m_atlasWidth(1024)
    , m_atlasHeight(512)
    , m_lineHeight(24.0f)
    , m_initialized(false) {
}

FontAtlas::~FontAtlas() {
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
    }
}

void FontAtlas::init(const char* fontPath) {
    if (m_initialized) return;

    generateFontAtlas(fontPath);
    m_initialized = true;
}

const GlyphInfo& FontAtlas::getGlyph(char c) const {
    auto it = m_glyphs.find(c);
    if (it != m_glyphs.end()) {
        return it->second;
    }

    // Return space character as fallback
    static GlyphInfo fallback{};
    return fallback;
}

void FontAtlas::generateFontAtlas(const char* fontPath) {
    // Load font file from disk
    std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "FontAtlas: Failed to open font file: " << fontPath << std::endl;
        return;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> fontBuffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(fontBuffer.data()), fileSize)) {
        std::cerr << "FontAtlas: Failed to read font file: " << fontPath << std::endl;
        return;
    }
    file.close();

    std::cout << "FontAtlas: Loaded font file " << fontPath << " (" << fileSize << " bytes)" << std::endl;

    // Prepare bitmap buffer for atlas
    unsigned char* bitmap = new unsigned char[m_atlasWidth * m_atlasHeight];
    std::memset(bitmap, 0, m_atlasWidth * m_atlasHeight);

    // Prepare baked char data
    const int firstChar = 32;  // Space
    const int numChars = 95;   // ASCII 32-126
    stbtt_bakedchar charData[numChars];

    // Bake font into bitmap
    // Higher resolution = sharper text when scaled up
    float pixelHeight = 48.0f;  // Font size in pixels
    int result = stbtt_BakeFontBitmap(
        fontBuffer.data(),                     // Font data
        0,                                      // Font offset (0 for first font in file)
        pixelHeight,                            // Pixel height
        bitmap,                                 // Output bitmap
        m_atlasWidth,                          // Bitmap width
        m_atlasHeight,                         // Bitmap height
        firstChar,                             // First character
        numChars,                              // Number of characters
        charData                               // Output character data
    );

    if (result <= 0) {
        std::cerr << "FontAtlas: Failed to bake font bitmap" << std::endl;
        delete[] bitmap;
        return;
    }

    // Create OpenGL texture
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // Upload bitmap to texture (single channel)
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        m_atlasWidth,
        m_atlasHeight,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap
    );

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up bitmap
    delete[] bitmap;

    // Convert stbtt_bakedchar to GlyphInfo
    // Note: stb_truetype uses top-left origin, but we need to flip V for OpenGL
    for (int i = 0; i < numChars; i++) {
        char c = static_cast<char>(firstChar + i);
        const stbtt_bakedchar& bc = charData[i];

        GlyphInfo glyph;

        // UV coordinates (normalized to 0-1)
        glyph.uvMin.x = bc.x0 / static_cast<float>(m_atlasWidth);
        glyph.uvMin.y = bc.y0 / static_cast<float>(m_atlasHeight);
        glyph.uvMax.x = bc.x1 / static_cast<float>(m_atlasWidth);
        glyph.uvMax.y = bc.y1 / static_cast<float>(m_atlasHeight);

        // Glyph size in pixels
        glyph.size.x = bc.x1 - bc.x0;
        glyph.size.y = bc.y1 - bc.y0;

        // Offset from baseline
        glyph.offset.x = bc.xoff;
        glyph.offset.y = bc.yoff;

        // Horizontal advance
        glyph.advance = bc.xadvance;

        m_glyphs[c] = glyph;
    }

    m_lineHeight = pixelHeight;

    std::cout << "FontAtlas: Successfully generated " << numChars
              << " glyphs at " << m_atlasWidth << "x" << m_atlasHeight << std::endl;
}

} // namespace delphinis
