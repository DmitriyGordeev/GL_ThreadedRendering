#ifndef COLLIDERGAME_SCENE_H
#define COLLIDERGAME_SCENE_H

#include <map>
#include <memory>
#include "Object.h"

class Scene {
public:
    Scene();
    virtual ~Scene();

    /** Adds object to the scene, returning new ID that Scene has assigned to it */
    unsigned long add(const std::shared_ptr<Object>& object);

    /** Renders all objects that Scene holds */
    void render();

protected:
    std::map<unsigned long, std::shared_ptr<Object>> m_Objects;
    unsigned long m_CurrentID {0};
};


#endif //COLLIDERGAME_SCENE_H
