#include "Object.h"
#include "Vertex.h"

Object::Object() {

}

Object::~Object() {

}

void Object::buildBuffers() {

    Vertex topLeft {};
    topLeft.pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    topLeft.pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    topLeft.color.r = 255;
    topLeft.color.g = 255;
    topLeft.color.b = 255;
    topLeft.uv.u = 0.0f;
    topLeft.uv.v = 1.0f;

    Vertex topRight {};
    topRight.pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    topRight.pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    topRight.color.r = 255;
    topRight.color.g = 255;
    topRight.color.b = 255;
    topRight.uv.u = 1.0f;
    topRight.uv.v = 1.0f;

    Vertex botRight {};
    botRight.pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    botRight.pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    botRight.color.r = 255;
    botRight.color.g = 255;
    botRight.color.b = 255;
    botRight.uv.u = 1.0f;
    botRight.uv.v = 0.0f;

    Vertex botLeft {};
    botLeft.pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    botLeft.pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    botLeft.color.r = 255;
    botLeft.color.g = 255;
    botLeft.color.b = 255;
    botLeft.uv.u = 0.0f;
    botLeft.uv.v = 0.0f;

    Vertex data[] = {topLeft, topRight, botRight, botLeft };


    if  (m)




    if (m_VaoID == 0)
        glGenVertexArrays(1, &m_VaoID);
    glBindVertexArray(m_VaoID);

    if(m_VboID == 0)
        glGenBuffers(1, &m_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
//    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
//
//    glBindVertexArray(0);
}