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
    float height{0.5f};             // Text height in world units
    TextAlign align{TextAlign::Left};  // Horizontal alignment

    Text() = default;
    Text(const std::string& str) : content(str) {}
    Text(const std::string& str, Vec3 col) : content(str), color(col) {}
    Text(const std::string& str, Vec3 col, float h) : content(str), color(col), height(h) {}
    Text(const std::string& str, Vec3 col, float h, TextAlign a) : content(str), color(col), height(h), align(a) {}
};

} // namespace delphinis
