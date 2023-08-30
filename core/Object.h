#ifndef COLLIDERGAME_OBJECT_H
#define COLLIDERGAME_OBJECT_H

#include "Geometry.h"
#include <gl/glew.h>
#include <glm.hpp>

class Object {
public:
    Object();
    virtual ~Object();

protected:
    void buildBuffers();

protected:
    GLuint m_VboID;
    GLuint m_VaoID;

    glm::vec2 m_Position {0.0f, 0.0f};
    glm::vec2 m_WorldSize {1.0f, 1.0f};
};


#endif //COLLIDERGAME_OBJECT_H
