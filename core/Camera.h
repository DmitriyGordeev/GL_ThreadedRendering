
#ifndef COLLIDERGAME_CAMERA_H
#define COLLIDERGAME_CAMERA_H

#include <glm.hpp>

class Camera {
public:
    Camera();
    virtual ~Camera();

    void init(int screenWidth, int screenHeight);
    void update();

    void setPosition(const glm::vec2& newPos);
    void setScale(float newScale);
    glm::vec2 getPos();
    [[nodiscard]] float getScale() const;
    [[nodiscard]] glm::mat4 getCameraMatrix() const;

    void toWorld(glm::vec2& screenCoords);


protected:
    bool m_NeedMatrixUpdate;
    float m_Scale;
    glm::vec2 m_Pos;
    glm::mat4 m_CameraMatrix;
    glm::mat4 m_OrthMatrix;
    int m_ScreenWidth;
    int m_ScreenHeight;
};


#endif //COLLIDERGAME_CAMERA_H
