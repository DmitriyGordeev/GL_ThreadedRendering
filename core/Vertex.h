#ifndef COLLIDERGAME_VERTEX_H
#define COLLIDERGAME_VERTEX_H

#include <gl/glew.h>

struct Pos {
    float x {0.0f};
    float y {0.0f};
};

struct Color {
    GLubyte r {0};
    GLubyte g {0};
    GLubyte b {0};
    GLubyte a {255};
};

struct UV {
    float u {0.0f};
    float v {0.0f};
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
