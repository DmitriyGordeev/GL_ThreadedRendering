#include "Shaders.h"
#include "Logger.h"
#include "Vertex.h"
#include <vector>
#include <fstream>
#include "TextureLoader.h"

Shaders::Shaders() = default;

Shaders::~Shaders() = default;


void Shaders::compile(const std::string& vertexShader, const std::string& pixelShader)
{
    // Create empty program handle
    m_ShaderProgramID = glCreateProgram();

    // Create vertex shader
    m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (m_VertexShaderID == 0)
        Logger::info("Error: Vertex Shader failed to be created!");

    // Create fragment shader
    m_FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_FragShaderID == 0)
        Logger::info("Error: Pixel Shader failed to be created!");

    // Compile shaders
    compileSingleShader(vertexShader, m_VertexShaderID);
    compileSingleShader(pixelShader, m_FragShaderID);
}

void Shaders::link() const
{
    // Attach created shaders to program
    glAttachShader(m_ShaderProgramID, m_VertexShaderID);
    glAttachShader(m_ShaderProgramID, m_FragShaderID);

    // Linking program
    glLinkProgram(m_ShaderProgramID);

    // Handle linking error
    GLint isLinked = 0;
    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_ShaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_ShaderProgramID, maxLength, &maxLength, &infoLog[0]);

        // Don't need the program anymore
        glDeleteProgram(m_ShaderProgramID);

        // Deleting shaders
        glDeleteShader(m_VertexShaderID);
        glDeleteShader(m_FragShaderID);

        // Print logged error message
        std::printf("%s\n", &(infoLog[0]));
        throw std::runtime_error("Shaders failed to link");
    }

    // Detaching shader after a successful link!
    glDetachShader(m_ShaderProgramID, m_VertexShaderID);
    glDetachShader(m_ShaderProgramID, m_FragShaderID);
    glDeleteShader(m_VertexShaderID);
    glDeleteShader(m_FragShaderID);
}

void Shaders::setupAttributes() {
    m_PosAttribID = glGetAttribLocation(m_ShaderProgramID, "vertexPosition");
    // second arg = 2 because we draw in 2D
    // positions block comes first - offset is 0: (void*)0
    glVertexAttribPointer(m_PosAttribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(m_PosAttribID);

    // color attributes, comes after position 2 float = 8 bytes: (void*)8
    m_ColorAttribID = glGetAttribLocation(m_ShaderProgramID, "vertexColor");
    glVertexAttribPointer(m_ColorAttribID, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)8);
    glEnableVertexAttribArray(m_ColorAttribID);

    // uv atrributes, comes after position (2 floats) and
    // 4 colors (4 bytes) = 8 + 4 = 12: (void*)12
    m_UVAttribID = glGetAttribLocation(m_ShaderProgramID, "vertexUV");
    glVertexAttribPointer(m_UVAttribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(m_UVAttribID);
}

bool Shaders::loadTexture(const std::string& path) {

    m_TextureID = TextureLoader::loadTexture(path);
    return (m_TextureID != 0);
}

GLuint Shaders::getUniformLocation(const std::string& uniformName) const
{
    GLuint location = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
    if (location == GL_INVALID_INDEX)
        Logger::error("Uniform variable '" + uniformName + "' not found in shader");

    return location;
}

void Shaders::compileSingleShader(const std::string& shaderFilename, GLuint id)
{
    std::ifstream shaderFile(shaderFilename);
    if (shaderFile.fail())
        Logger::error("Error: Failed to open " + shaderFilename);

    std::string fileContent;
    std::string line;
    while (std::getline(shaderFile, line))
        fileContent += line + "\n";
    shaderFile.close();

    // Load shader to OpenGL and compile
    const char* contentsPtr = fileContent.c_str();
    glShaderSource(id, 1, &contentsPtr, nullptr);
    glCompileShader(id);

    // Handle errors
    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

        glDeleteShader(id);
        std::printf("%s\n", &(errorLog[0]));
        throw std::runtime_error("Shader" + shaderFilename + " failed to compile");
    }
}