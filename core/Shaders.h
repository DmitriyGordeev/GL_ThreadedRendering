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

    void setupAttributes();

    [[nodiscard]] GLuint getUniformLocation(const std::string& uniformName) const;

    void use() const;
    void disable() const;

    [[nodiscard]] GLuint getShaderProgramId() const { return m_ShaderProgramID; }

protected:
    static void compileSingleShader(const std::string& shaderFilename, GLuint id);

protected:
    GLuint m_ShaderProgramID {0};
    GLuint m_VertexShaderID {0};
    GLuint m_FragShaderID {0};

    GLuint m_PosAttribID {0};
    GLuint m_ColorAttribID {0};
    GLuint m_UVAttribID {0};
};


#endif //COLLIDERGAME_SHADERS_H
