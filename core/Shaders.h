#ifndef COLLIDERGAME_SHADERS_H
#define COLLIDERGAME_SHADERS_H

#include <gl/glew.h>
#include <string>
#include <vector>
#include <future>

class Shaders {
public:
    Shaders(std::string vertexShader, std::string fragmentShader);
    ~Shaders();

    void compile();
    void link() const;

    /** Tells opengl how vertex attributes (pos, color, uv, ...)
    are aligned in a memory block of a single vertex */
    void setupAttributes();

    bool loadTexture(const std::string& path);

    [[nodiscard]] GLuint getUniformLocation(const std::string& uniformName) const;
    [[nodiscard]] GLuint getShaderProgramId() const { return m_ShaderProgramID; }

    [[nodiscard]] GLuint getPositionAttrID() const { return m_PosAttribID; }
    [[nodiscard]] GLuint getColorAttributeID() const { return m_ColorAttribID; }
    [[nodiscard]] GLuint getUVAttributeID() const { return m_UVAttribID; }

    [[nodiscard]] GLuint getTextureID() const { return m_TextureID; }

    void blockUntilShaderLoaded();

    std::promise<bool> m_ShaderLoadedPromise;

protected:
    static void compileSingleShader(const std::string& shaderFilename, GLuint id);

protected:
    std::string m_VertexShaderFilePath;
    std::string m_FragmentShaderFilePath;

    GLuint m_ShaderProgramID {0};
    GLuint m_VertexShaderID {0};
    GLuint m_FragShaderID {0};

    GLint m_PosAttribID {-1};
    GLint m_ColorAttribID {-1};
    GLint m_UVAttribID {-1};

    GLuint m_TextureID {0};


//    std::promise<bool> m_ShaderTextureLoadedPromise;
};


#endif //COLLIDERGAME_SHADERS_H
