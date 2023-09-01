#include "Scene.h"

Scene::Scene() = default;

Scene::~Scene() = default;

unsigned long Scene::add(const std::shared_ptr<Object>& object) {
    if (!object)
        return 0;

    m_CurrentID++;
    object->setObjectID(m_CurrentID);
    m_Objects.emplace(m_CurrentID, object);
    return m_CurrentID;
}

void Scene::render() {
    for(auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
        itr->second->render();
}