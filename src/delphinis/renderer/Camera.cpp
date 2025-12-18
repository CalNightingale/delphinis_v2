#include "delphinis/renderer/Camera.h"

namespace delphinis {

Camera::Camera(float width, float height)
    : m_width(width), m_height(height) {
    updateProjection();
}

void Camera::setViewport(float width, float height) {
    m_width = width;
    m_height = height;
    updateProjection();
}

const Mat4& Camera::getProjectionMatrix() const {
    return m_projection;
}

void Camera::updateProjection() {
    // Create orthographic projection centered at origin
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;
    m_projection = Mat4::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
}

} // namespace delphinis
