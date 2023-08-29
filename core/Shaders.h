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
    void addAttribute(const std::string& attributeName);

    [[nodiscard]] GLuint getUniformLocation(const std::string& uniformName) const;

    void use() const;
    void disable() const;

protected:
    static void compileSingleShader(const std::string& shaderFilename, GLuint id);

protected:
    int m_NumAttributes;
    GLuint m_ShaderProgramID;
    GLuint m_VertexShaderID;
    GLuint m_FragShaderID;
};


#endif //COLLIDERGAME_SHADERS_H
