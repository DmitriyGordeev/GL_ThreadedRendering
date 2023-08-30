#ifndef COLLIDERGAME_VERTEX_H
#define COLLIDERGAME_VERTEX_H

#include <gl/glew.h>

struct Pos {
    float x;
    float y;
};

struct Color {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct UV {
    float u;
    float v;
};

struct Vertex {
    Pos pos;
    Color color;
    UV uv;

    void setPos(float x, float y) {
        pos.x = x;
        pos.y = y;
    }

    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void setUV(float u, float v) {
        uv.u = u;
        uv.v = v;
    }
};

#endif //COLLIDERGAME_VERTEX_H
