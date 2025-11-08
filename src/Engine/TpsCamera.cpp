#include <Engine/TpsCamera.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Engine/Math/Transform.hpp>
#include <OpenGL.hpp>
#include <iostream>
#include <Engine/Math/Math.hpp>

namespace Engine
{
    TpsCamera::TpsCamera(float FOV) : Camera(FOV) {

    };

    glm::mat4 TpsCamera::getViewMatrix() {
        glm::vec3 position = this->transform.getPosition();
        glm::quat rotation = this->transform.getRotation();

        // Obliczamy kierunek, w który patrzy kamera
        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up    = rotation * glm::vec3(0.0f, 1.0f,  0.0f);

        return glm::lookAt(position, position + front, up);
    }


    float TpsCamera::getFOV() {
        return this->FOV;
    };

    void TpsCamera::handleMouseInput(GLFWwindow * window, float deltaTime) {
        static bool init = false;
        static float lastXpos = 0.0;
        static float lastYpos = 0.0; 

        static float yaw = 0.0f;
        static float pitch = 0.0f;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!init) {
            init = true;
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            lastXpos = xpos;
            lastYpos = ypos;
            return;
        }

        float sensitivity = 0.07f;

        float dx = (lastXpos - xpos);
        float dy = (lastYpos - ypos);

        yaw = yaw + sensitivity * dx;
        pitch = std::min(89.0f, std::max(-89.0f, pitch + sensitivity * dy));

        glm::quat sidewaysRotation = glm::angleAxis(
            glm::radians(yaw),
            glm::vec3(0, 1, 0)
        );
        
        glm::quat upwardsRotation = glm::angleAxis(
            glm::radians(pitch),
            glm::vec3(1, 0, 0)
        );

        // rotate the forward vector and set our camera position to target - forward * distance
        // and then rotate the camera to look at our player

        glm::vec3 forward = {0.0f, 0.0f,  -1.0f};
        forward = sidewaysRotation * upwardsRotation * forward;
        float distance = 35.0f;

        this->transform.setPosition(
            targetPosition - forward * distance
        );

        this->transform.setRotation(
            Math::lookAtQuat(this->transform.getPosition(), targetPosition, {0.0f, 1.0f, 0.0f})
        );
        
        lastXpos = xpos;
        lastYpos = ypos;
    };

    void TpsCamera::syncCameraAndTarget(Math::Transform& target) {
        targetPosition = target.getPosition();
    };

    // TODO: Fix
    glm::vec3 TpsCamera::getForward() { 
        auto temp = this->transform.getRotation() * glm::vec3(0.0f, 0.0f,  -1.0f); 
        temp.y = 0;
        return glm::normalize(temp);
    };

    glm::vec3 TpsCamera::getUp() { 
        return glm::vec3(0.0f, 1.0f,  0.0f); 
    };

    glm::vec3 TpsCamera::getRight() { 
        auto temp = this->transform.getRotation() * glm::vec3(1.0f, 0.0f,  0.0f); 
        temp.y = 0;
        return glm::normalize(temp);
    };
};