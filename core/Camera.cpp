#include "Camera.h"
#include <gtc\matrix_transform.hpp>

Camera::Camera() : m_NeedMatrixUpdate(true),
                   m_Scale(0.3f),
                   m_Pos(0.0f, 0.0f),
                   m_CameraMatrix(1.0f),
                   m_OrthMatrix(1.0f),
                   m_ScreenWidth(640),          // todo: feed width and height from window object
                   m_ScreenHeight(480) {}

Camera::~Camera() = default;

void Camera::init(int screenWidth, int screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
    m_OrthMatrix = glm::ortho(0.0f,
                              (float) m_ScreenWidth,
                              0.0f,
                              (float) m_ScreenHeight);

}

void Camera::update() {
    if (m_NeedMatrixUpdate)
    {
        glm::vec3 translate(-m_Pos.x + (float)m_ScreenHeight / 2,
                            -m_Pos.y + (float)m_ScreenHeight / 2,
                            0.0f);

        m_CameraMatrix = glm::translate(m_OrthMatrix, translate);

        glm::vec3 scale(m_Scale, m_Scale, 0.0f);
        m_CameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_CameraMatrix;

        //translate = glm::vec3(_screenWidth / 2, _screenHeight / 2, 0.0f);
        m_NeedMatrixUpdate = false;
    }
}

void Camera::setPosition(const glm::vec2 &newPos) {
    m_Pos = newPos;
    m_NeedMatrixUpdate = true;
}

void Camera::setScale(float newScale) {
    m_Scale = newScale;
    m_NeedMatrixUpdate = true;
}

glm::vec2 Camera::getPos() {
    return m_Pos;
}

float Camera::getScale() const {
    return m_Scale;
}

glm::mat4 Camera::getCameraMatrix() const {
    return m_CameraMatrix;
}

void Camera::toWorld(glm::vec2 &screenCoords) {
    screenCoords -= glm::vec2(m_ScreenWidth / 2, m_ScreenHeight / 2);
    screenCoords /= m_Scale;
    screenCoords += m_Pos;
}