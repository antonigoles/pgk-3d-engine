#include <Engine/Player.hpp>
#include <Engine/Math/Math.hpp>
#include <OpenGL.hpp>

namespace Engine {
    Player::Player() {
        transform = Math::Transform();
    }

    void Player::handleKeyboardInput(GLFWwindow * window, Camera* camera, float deltaTime) {
        static float deltaFOV = 0;
        static float velocity = 0.0f;
        static glm::vec3 direction = glm::vec3{0.0f, 0.0f, 0.0f};
        
        const float acceleration = 1.0f;
        const float deacceleration = 2.0f;
        const float maxVelocity = 2.0f;

        glm::vec3 playerPos = this->transform.getPosition();
        glm::vec3 temporaryDirection = glm::vec3{0.0f, 0.0f, 0.0f};

        bool input = false;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            temporaryDirection += camera->getForward();
            input=true;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            temporaryDirection -= camera->getForward();
            input=true;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            temporaryDirection -= camera->getRight();
            input=true;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            temporaryDirection += camera->getRight();
            input=true;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            temporaryDirection += camera->getUp();
            input=true;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            temporaryDirection -= camera->getUp();
            input=true;
        }

        // temporaryDirection.y = 0;

        temporaryDirection = glm::length(temporaryDirection) == 0.0 
            ? glm::vec3{0.0f,0.0f,0.0f} : glm::normalize(temporaryDirection);

        if (temporaryDirection != glm::vec3{0,0,0}) {
            direction = temporaryDirection;
        }

        velocity = input ? velocity + acceleration * deltaTime : velocity - deacceleration * deltaTime;
        velocity = std::min(maxVelocity, std::max(0.0f, velocity));

        this->transform.setPosition(playerPos + direction * velocity * deltaTime * 10.0f);
        this->playerGameObject->transform.setPosition(this->transform.getPosition() - glm::vec3{0, 2.0f, 0});
        if (velocity > 0.0) {
            this->playerGameObject->set_vec3("forward", direction);
        }
        this->playerGameObject->set_float("velocity", velocity);
    }

    void Player::setPlayerGameObject(GameObject* playerGameObject) {
        this->playerGameObject = playerGameObject;
    };
}