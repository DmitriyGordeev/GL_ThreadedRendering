#include "Object.h"
#include "Vertex.h"
#include "Logger.h"

Object::Object() {
    createGeometry();
}

Object::Object(const glm::vec2& pos, const glm::vec2& size):
    m_Position(pos),
    m_WorldSize(size) {
    createGeometry();
}

Object::~Object() {
    if (m_Geometry) {
        delete[] m_Geometry;
        m_Geometry = nullptr;
    }

    if (m_Indices) {
        delete[] m_Indices;
        m_Indices = nullptr;
    }
}

void Object::createGeometry() {
    m_Geometry = new Vertex[4];

    // top left
    m_Geometry[0].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    m_Geometry[0].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    m_Geometry[0].color.r = 0;
    m_Geometry[0].color.g = 20;
    m_Geometry[0].color.b = 0;
    m_Geometry[0].uv.u = 0.0f;
    m_Geometry[0].uv.v = 1.0f;

    // bottom left
    m_Geometry[1].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    m_Geometry[1].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[1].color.r = 255;
    m_Geometry[1].color.g = 255;
    m_Geometry[1].color.b = 255;
    m_Geometry[1].uv.u = 0.0f;
    m_Geometry[1].uv.v = 0.0f;

    // bottom right
    m_Geometry[2].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    m_Geometry[2].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[2].color.r = 255;
    m_Geometry[2].color.g = 255;
    m_Geometry[2].color.b = 255;
    m_Geometry[2].uv.u = 1.0f;
    m_Geometry[2].uv.v = 0.0f;

    // top right
    m_Geometry[3].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    m_Geometry[3].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    m_Geometry[3].color.r = 255;
    m_Geometry[3].color.g = 255;
    m_Geometry[3].color.b = 255;
    m_Geometry[3].uv.u = 1.0f;
    m_Geometry[3].uv.v = 1.0f;

    buildBuffers();
}

void Object::buildBuffers() {

    // 1. Create vertex array object
    if (m_VaoID == 0) {
        glGenVertexArrays(1, &m_VaoID);
        glBindVertexArray(m_VaoID);
    }

    // 2. Create vertex buffer object and bind our Vertex* data to its ID (m_VboID)
    // which OpenGL provided us with
    if(m_VboID == 0) {
        glGenBuffers(1, &m_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), m_Geometry, GL_STATIC_DRAW);
    }

    // 3. Create indices to define winding order
    m_Indices = new int[6];
    m_Indices[0] = 3;
    m_Indices[1] = 0;
    m_Indices[2] = 1;
    m_Indices[3] = 1;
    m_Indices[4] = 2;
    m_Indices[5] = 3;

    if(m_IboID == 0) {
        glGenBuffers(1, &m_IboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), m_Indices, GL_STATIC_DRAW);
    }

    // 4. Unbind buffers - mark that we've done everything we want with
    // binding points (vertex array, GL_ARRAY_BUFFER, GL_INDEX_BUFFER)
    // and detach, making further changes with
    // our data (m_Geometry, indices, ...) invisible for OpenGL's internal state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::applyShader(const std::shared_ptr<Shaders>& shader) {
    if (!shader) {
        Logger::info("Object::applyShader() shader is nullptr");
        return;
    }

    if (shader->getShaderProgramId() == 0) {
        Logger::info("Object::applyShader() shader program ID = 0");
        return;
    }

    // if shader's attributes have been initialized before
    if (shader->getPositionAttrID() != -1 ||
        shader->getColorAttributeID() != -1 ||
        shader->getUVAttributeID() != -1) {
        Logger::warning("Shader attributes have been initialized before");
        return;
    }

    // Save reference to associated material for this object
    m_ShaderRef = shader;

    glBindVertexArray(m_VaoID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);

    shader->setupAttributes();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::render() {
    if (!m_ShaderRef.lock()) {
        Logger::info("Object::render() - shader is null");
        return;
    }

    auto shader = m_ShaderRef.lock();
    glUseProgram(shader->getShaderProgramId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader->getTextureID());

    glBindVertexArray(m_VaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

