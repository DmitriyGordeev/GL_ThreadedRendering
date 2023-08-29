#include "InputSystem.h"


InputSystem::InputSystem() = default;

InputSystem::~InputSystem() = default;

void InputSystem::pressKey(unsigned int keyID) {
    m_KeyMap[keyID] = true;
}

void InputSystem::releaseKey(unsigned int keyID) {
    m_KeyMap[keyID] = false;
}

bool InputSystem::isKeyPressed(unsigned int keyID) {
    auto it = m_KeyMap.find(keyID);
    if (it != m_KeyMap.end())
        return it->second;
    else
        return false;
}

void InputSystem::setMouseCoords(float x, float y) {
    m_MouseCoords.x = x;
    m_MouseCoords.y = y;
}

glm::vec2 InputSystem::getMouseCoords() const { return m_MouseCoords; }