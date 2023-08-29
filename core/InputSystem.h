#ifndef COLLIDERGAME_INPUTSYSTEM_H
#define COLLIDERGAME_INPUTSYSTEM_H

#include <unordered_map>
#include <glm.hpp>

class InputSystem {
public:
    InputSystem();
    virtual ~InputSystem();

    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);

    bool isKeyPressed(unsigned int keyID);
    void setMouseCoords(float x, float y);

    [[nodiscard]] glm::vec2 getMouseCoords() const;

protected:
    std::unordered_map<unsigned int, bool> m_KeyMap;
    glm::vec2 m_MouseCoords {0.0f, 0.0f};
};


#endif //COLLIDERGAME_INPUTSYSTEM_H
