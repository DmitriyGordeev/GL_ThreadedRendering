#include "Object.h"
#include "Vertex.h"

Object::Object() {

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
    m_Geometry[0].color.r = 255;
    m_Geometry[0].color.g = 255;
    m_Geometry[0].color.b = 255;
    m_Geometry[0].uv.u = 0.0f;
    m_Geometry[0].uv.v = 1.0f;

    // top right
    m_Geometry[1].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    m_Geometry[1].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    m_Geometry[1].color.r = 255;
    m_Geometry[1].color.g = 255;
    m_Geometry[1].color.b = 255;
    m_Geometry[1].uv.u = 1.0f;
    m_Geometry[1].uv.v = 1.0f;

    // bottom right
    m_Geometry[2].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    m_Geometry[2].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[2].color.r = 255;
    m_Geometry[2].color.g = 255;
    m_Geometry[2].color.b = 255;
    m_Geometry[2].uv.u = 1.0f;
    m_Geometry[2].uv.v = 0.0f;

    // bottom left
    m_Geometry[3].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    m_Geometry[3].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[3].color.r = 255;
    m_Geometry[3].color.g = 255;
    m_Geometry[3].color.b = 255;
    m_Geometry[3].uv.u = 0.0f;
    m_Geometry[3].uv.v = 0.0f;

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
//    int vertexIndices[] = {3, 0, 1, 1, 2, 3};
    m_Indices[0] = 3;
    m_Indices[1] = 0;
    m_Indices[2] = 1;
    m_Indices[3] = 1;
    m_Indices[4] = 2;
    m_Indices[5] = 3;

    if(m_IboID == 0) {
        glGenBuffers(1, &m_IboID);
        glBindBuffer(GL_INDEX_ARRAY, m_IboID);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(int), m_Indices, GL_STATIC_DRAW);
    }

    // 4. Unbind buffers - mark that we've done everything we want with
    // binding points (vertex array, GL_ARRAY_BUFFER, GL_INDEX_BUFFER)
    // and detach, making further changes with
    // our data (m_Geometry, indices, ...) invisible for OpenGL's internal state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_INDEX_ARRAY, 0);
}

void Object::applyShader(GLuint shaderProgramID) {
    if (shaderProgramID == 0) {
        // todo: logger ? throw ?
        return;
    }

    glUseProgram(shaderProgramID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);

    // let know opengl how position values are layout inside Vertex* m_Geometry bytes
    m_PosAttribID = glGetAttribLocation(shaderProgramID, "coordinates");
    // second arg 2 because we draw in 2D space - for 3D need to replace with 3
    glVertexAttribPointer(m_PosAttribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    // color attributes
    m_ColorAttribID = glGetAttribLocation(shaderProgramID, "color");
    glVertexAttribPointer(m_ColorAttribID, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // uv atrributes
    m_UVAttribID = glGetAttribLocation(shaderProgramID, "uv");
    glVertexAttribPointer(m_UVAttribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::render(GLuint shaderProgramID) {
    glBindVertexArray(m_VaoID);
    glUseProgram(shaderProgramID);

    // enable shader attributes
    glEnableVertexAttribArray(m_PosAttribID);
    glEnableVertexAttribArray(m_ColorAttribID);
    glEnableVertexAttribArray(m_UVAttribID);

    // setTextureUniform();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, m_Indices);

    // disable vertex attributes
    glDisableVertexAttribArray(m_PosAttribID);
    glDisableVertexAttribArray(m_ColorAttribID);
    glDisableVertexAttribArray(m_UVAttribID);
    glBindVertexArray(0);
}


//void Object::buildBuffers(GLuint shaderProgram) {
//    // TODO: Geometry and 'GLuint texture' !
//
//    Vertex topLeft {};
//    topLeft.pos.x = m_Position.x - m_WorldSize.x / 2.0f;
//    topLeft.pos.y = m_Position.y + m_WorldSize.y / 2.0f;
//    topLeft.color.r = 255;
//    topLeft.color.g = 255;
//    topLeft.color.b = 255;
//    topLeft.uv.u = 0.0f;
//    topLeft.uv.v = 1.0f;
//
//    Vertex topRight {};
//    topRight.pos.x = m_Position.x + m_WorldSize.x / 2.0f;
//    topRight.pos.y = m_Position.y + m_WorldSize.y / 2.0f;
//    topRight.color.r = 255;
//    topRight.color.g = 255;
//    topRight.color.b = 255;
//    topRight.uv.u = 1.0f;
//    topRight.uv.v = 1.0f;
//
//    Vertex botRight {};
//    botRight.pos.x = m_Position.x + m_WorldSize.x / 2.0f;
//    botRight.pos.y = m_Position.y - m_WorldSize.y / 2.0f;
//    botRight.color.r = 255;
//    botRight.color.g = 255;
//    botRight.color.b = 255;
//    botRight.uv.u = 1.0f;
//    botRight.uv.v = 0.0f;
//
//    Vertex botLeft {};
//    botLeft.pos.x = m_Position.x - m_WorldSize.x / 2.0f;
//    botLeft.pos.y = m_Position.y - m_WorldSize.y / 2.0f;
//    botLeft.color.r = 255;
//    botLeft.color.g = 255;
//    botLeft.color.b = 255;
//    botLeft.uv.u = 0.0f;
//    botLeft.uv.v = 0.0f;
//
//    Vertex data[] = {topLeft, topRight, botRight, botLeft };
//
//    // todo: if haderProgramID == 0 -> throw / return ?
//
//    if (m_VaoID == 0)
//        glGenVertexArrays(1, &m_VaoID);
//    glBindVertexArray(m_VaoID);
//
//    if(m_VboID == 0)
//        glGenBuffers(1, &m_VboID);
//    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
//    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
//
//
//
//
//
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//
//
//
//
////
////    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
////    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
////    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
////
////    glBindVertexArray(0);
//}