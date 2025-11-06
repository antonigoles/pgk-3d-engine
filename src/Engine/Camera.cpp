#include <Engine/Camera.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Engine/Math/Transform.hpp>
#include <OpenGL.hpp>
#include <iostream>

namespace Engine
{
    Camera::Camera(float FOV) {
        this->FOV = FOV;
        transform = Math::Transform();
    };

    glm::mat4 Camera::getViewMatrix() {
        glm::vec3 position = this->transform.getPosition();
        glm::quat rotation = this->transform.getRotation();

        // Obliczamy kierunek, w który patrzy kamera
        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up    = rotation * glm::vec3(0.0f, 1.0f,  0.0f);

        return glm::lookAt(position, position + front, up);
    }


    float Camera::getFOV() {
        return this->FOV;
    };

    void Camera::handleMouseInput(GLFWwindow * window, float deltaTime) {
        static bool init = false;
        static float lastXpos = 0.0;
        static float lastYpos = 0.0; 

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!init) {
            init = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            lastXpos = xpos;
            lastYpos = ypos;
        }

        float sensitivity = 0.1f;

        float dx = (lastXpos - xpos);
        float dy = (lastYpos - ypos);

        float yawAngle   = glm::radians(dx * sensitivity);
        float pitchAngle = glm::radians(dy * sensitivity);

        auto rotation = this->transform.getRotation();

        glm::quat qYaw = glm::angleAxis(yawAngle, glm::vec3(0, 1, 0));
        rotation = glm::normalize(qYaw * rotation);

        glm::vec3 right = rotation * glm::vec3(1, 0, 0);
        glm::quat qPitch = glm::angleAxis(pitchAngle, right);
        glm::quat newRot = glm::normalize(rotation * qPitch);
        
        glm::vec3 forward = newRot * glm::vec3(0, 0, -1);
        float dotUp = glm::dot(forward, glm::vec3(0, 1, 0));
        if (dotUp > 0.98f || dotUp < -0.98f) {
            // za blisko góry/dół – nie stosuj tej rotacji
            this->transform.setRotation(rotation);
        } else {
            this->transform.setRotation(newRot);
        }


        glm::vec3 cameraPos = this->transform.getPosition();
        glm::vec3 velocity = glm::vec3{0.0f, 0.0f, 0.0f};

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            velocity.z -= 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            velocity.z += 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            velocity.x -= 1.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            velocity.x += 1.0f;
        }

        velocity = this->transform.getRotation() * velocity;

        velocity.y = 0;

        velocity = glm::length(velocity) == 0.0 
            ? glm::vec3{0.0f,0.0f,0.0f} : glm::normalize(velocity);

        this->transform.setPosition(cameraPos + velocity * deltaTime * 10.0f);

        lastXpos = xpos;
        lastYpos = ypos;
    };
};