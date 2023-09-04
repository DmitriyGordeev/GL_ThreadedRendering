#include "Scene.h"

Scene::Scene() = default;

Scene::~Scene() = default;

unsigned long Scene::add(const std::shared_ptr<Object>& object) {
    if (!object)
        return 0;

    m_GlobalObjectCounterID++;
    object->setObjectID(m_GlobalObjectCounterID);
    m_Objects.emplace(m_GlobalObjectCounterID, object);
    return m_GlobalObjectCounterID;
}

void Scene::render() {
    for(auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
        itr->second->render();
}

