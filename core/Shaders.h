#ifndef COLLIDERGAME_SHADERS_H
#define COLLIDERGAME_SHADERS_H

#include <gl/glew.h>
#include <string>

class Shaders {
public:
    Shaders();
    ~Shaders();

    void compile(const std::string& vertexShader, const std::string& pixelShader);
    void link() const;

    /** Tells opengl how vertex attributes (pos, color, uv, ...)
    are aligned in a memory block of a single vertex */
    void setupAttributes();

    [[nodiscard]] GLuint getUniformLocation(const std::string& uniformName) const;
    [[nodiscard]] GLuint getShaderProgramId() const { return m_ShaderProgramID; }

    [[nodiscard]] GLuint getPositionAttrID() const { return m_PosAttribID; }
    [[nodiscard]] GLuint getColorAttributeID() const { return m_ColorAttribID; }
    [[nodiscard]] GLuint getUVAttributeID() const { return m_UVAttribID; }

protected:
    static void compileSingleShader(const std::string& shaderFilename, GLuint id);

protected:
    GLuint m_ShaderProgramID {0};
    GLuint m_VertexShaderID {0};
    GLuint m_FragShaderID {0};

    GLint m_PosAttribID {-1};
    GLint m_ColorAttribID {-1};
    GLint m_UVAttribID {-1};
};


#endif //COLLIDERGAME_SHADERS_H
