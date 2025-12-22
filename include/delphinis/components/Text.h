#pragma once

#include <string>
#include "delphinis/components/Sprite.h"

namespace delphinis {

enum class TextAlign {
    Left,
    Center,
    Right
};

struct Text {
    std::string content;
    Vec3 color{1.0f, 1.0f, 1.0f};  // RGB tint
    float scale{1.0f};              // Size multiplier
    TextAlign align{TextAlign::Left};  // Horizontal alignment

    Text() = default;
    Text(const std::string& str) : content(str) {}
    Text(const std::string& str, Vec3 col) : content(str), color(col) {}
    Text(const std::string& str, Vec3 col, float s) : content(str), color(col), scale(s) {}
    Text(const std::string& str, Vec3 col, float s, TextAlign a) : content(str), color(col), scale(s), align(a) {}
};

} // namespace delphinis
