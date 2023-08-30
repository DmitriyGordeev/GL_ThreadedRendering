#ifndef COLLIDERGAME_OBJECT_H
#define COLLIDERGAME_OBJECT_H

#include <gl/glew.h>
#include <glm.hpp>
#include "Vertex.h"

class Object {
public:
    Object();

    virtual ~Object();

    void createGeometry();

    void applyShader(GLuint shaderProgramID);

    void render(GLuint shaderProgramID);

protected:
    void buildBuffers();

protected:
    GLuint m_VaoID {0};
    GLuint m_VboID {0};
    GLuint m_IboID {0};

    GLuint m_PosAttribID {0};
    GLuint m_ColorAttribID {0};
    GLuint m_UVAttribID {0};

//    GLuint m_TextureID {0};

    glm::vec2 m_Position {0.0f, 0.0f};
    glm::vec2 m_WorldSize {1.0f, 1.0f};

    Vertex* m_Geometry;     // todo: smart ptr ?
    int* m_Indices;         // todo: smart ptr ?
};


#endif //COLLIDERGAME_OBJECT_H
