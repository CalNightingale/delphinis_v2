#pragma once

#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include <string>
#include "delphinis/math/Mat4.h"
#include "delphinis/components/Sprite.h"

namespace delphinis {

class Shader {
public:
    Shader();
    ~Shader();

    void loadFromSource(const char* vertexSource, const char* fragmentSource);
    void use() const;

    void setMat4(const char* name, const Mat4& mat) const;
    void setVec3(const char* name, const Vec3& vec) const;
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;

    GLuint getProgram() const { return m_program; }

private:
    GLuint m_program;

    GLuint compileShader(GLenum type, const char* source);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);
};

} // namespace delphinis
