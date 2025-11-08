#include <Engine/Camera.hpp>

namespace Engine
{
    Camera::Camera(float FOV) {
        this->FOV = FOV;
        transform = Math::Transform();
    };

    glm::mat4 Camera::getViewMatrix() {
        return glm::mat4{};
    };
    float Camera::getFOV() {
        return this->FOV;
    };

    void Camera::handleMouseInput(GLFWwindow * window, float deltaTime) {};
    void Camera::syncCameraAndTarget(Math::Transform& target) {};

    glm::vec3 Camera::getForward() { return {0,0,-1.0f}; };
    glm::vec3 Camera::getUp() { return {0,1.0f,0}; };
    glm::vec3 Camera::getRight() { return {1.0f,0,0}; };
};