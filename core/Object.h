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

    void move(const glm::vec2& offsetPos);

    void applyShader(const std::shared_ptr<Shaders>& shader);

    void render();

    void setObjectID(unsigned long id) { m_ObjectID = id; }

    void buildBuffers();

    void updateBuffers();

protected:
    void createGeometry();

protected:
    GLuint m_VaoID {0};
    GLuint m_VboID {0};
    GLuint m_IboID {0};

    glm::vec2 m_Position {0.0f, 0.0f};
    glm::vec2 m_WorldSize {100.0f, 100.0f};

    Vertex* m_Geometry{nullptr};     // todo: smart ptr ?
    int* m_Indices{nullptr};         // todo: smart ptr ?

    /* current object's associated material */
    std::weak_ptr<Shaders> m_ShaderRef;

    unsigned long m_ObjectID {0};
    bool m_NeedUpdateBuffers {false};
};


#endif //COLLIDERGAME_ROBJECT_H
