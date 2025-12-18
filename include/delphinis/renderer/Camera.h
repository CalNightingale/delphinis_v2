#pragma once

#include "delphinis/math/Mat4.h"

namespace delphinis {

class Camera {
public:
    Camera(float width, float height);

    void setViewport(float width, float height);
    const Mat4& getProjectionMatrix() const;

private:
    float m_width;
    float m_height;
    Mat4 m_projection;

    void updateProjection();
};

} // namespace delphinis
