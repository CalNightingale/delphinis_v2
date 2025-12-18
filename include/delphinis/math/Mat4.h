#pragma once

#include <cstring>

namespace delphinis {

struct Mat4 {
    float m[16];  // Column-major order (OpenGL convention)

    Mat4() {
        identity();
    }

    void identity() {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
        Mat4 result;
        result.m[0] = 2.0f / (right - left);
        result.m[5] = 2.0f / (top - bottom);
        result.m[10] = -2.0f / (far - near);
        result.m[12] = -(right + left) / (right - left);
        result.m[13] = -(top + bottom) / (top - bottom);
        result.m[14] = -(far + near) / (far - near);
        result.m[15] = 1.0f;
        return result;
    }

    static Mat4 translate(float x, float y) {
        Mat4 result;
        result.m[12] = x;
        result.m[13] = y;
        return result;
    }

    static Mat4 scale(float x, float y) {
        Mat4 result;
        result.m[0] = x;
        result.m[5] = y;
        return result;
    }

    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                float sum = 0.0f;
                for (int k = 0; k < 4; k++) {
                    sum += m[k * 4 + row] * other.m[col * 4 + k];
                }
                result.m[col * 4 + row] = sum;
            }
        }
        return result;
    }

    const float* data() const { return m; }
};

} // namespace delphinis
