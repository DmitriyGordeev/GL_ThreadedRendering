#ifndef COLLIDERGAME_OBJECT_H
#define COLLIDERGAME_OBJECT_H

#include <gl/glew.h>
#include <glm.hpp>
#include "Vertex.h"
#include "Shaders.h"
#include <memory>

class Object {
public:
    Object();
    Object(const glm::vec2& pos, const glm::vec2& size);

    virtual ~Object();

    void createGeometry();

    void applyShader(const std::shared_ptr<Shaders>& shader);

    void render();

protected:
    void buildBuffers();

protected:
    GLuint m_VaoID {0};
    GLuint m_VboID {0};
    GLuint m_IboID {0};

//    GLuint m_TextureID {0};

    glm::vec2 m_Position {0.0f, 0.0f};
    glm::vec2 m_WorldSize {100.0f, 100.0f};

    Vertex* m_Geometry{nullptr};     // todo: smart ptr ?
    int* m_Indices{nullptr};         // todo: smart ptr ?

    /* current object's associated material */
    std::weak_ptr<Shaders> m_ShaderRef;
};


#endif //COLLIDERGAME_OBJECT_H
