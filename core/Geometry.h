#ifndef COLLIDERGAME_OBJECT_H
#define COLLIDERGAME_OBJECT_H

#include "Vertex.h"

struct Geometry {
    Vertex topLeft;
    Vertex topRight;
    Vertex bottomLeft;
    Vertex bottomRight;

    float depth;
    GLuint texture;
};


#endif //COLLIDERGAME_OBJECT_H
