#ifndef COLLIDERGAME_SCENE_H
#define COLLIDERGAME_SCENE_H

#include <map>
#include <memory>
#include "Object.h"
#include "Logger.h"

class Scene {
public:
    Scene();
    virtual ~Scene();

    /** Adds object to the scene, returning new ID that Scene has assigned to it */
    unsigned long add(const std::shared_ptr<Object>& object);

    /** Renders all objects that Scene holds */
    void render();

    /** Acts like an Object factory:
     * constructs object with perfect forwarding mechanism and
     * adds to the Scene's map(id -> Object ref) */
    template <class O, class ...Args>
    unsigned long createObject(const std::shared_ptr<Shaders>& shader,
                                      Args&& ...args) {
        if (!shader) {
            Logger::error("[Scene::createObject] shader is null");
            return 0;
        }

        shader->blockUntilShaderLoaded();
        Logger::info("Shader has been loaded, set to Object");

        // todo: std::forward<Args>
        std::shared_ptr<Object> pObj = std::make_shared<O>(args...);
        pObj->setShader(shader);
        return add(pObj);
    }

    [[nodiscard]] const std::map<unsigned long, std::shared_ptr<Object>>& getObjectsMap() const {
        return m_Objects;
    }

protected:
    std::map<unsigned long, std::shared_ptr<Object>> m_Objects;
    unsigned long m_GlobalObjectCounterID {0};
};


#endif //COLLIDERGAME_SCENE_H
