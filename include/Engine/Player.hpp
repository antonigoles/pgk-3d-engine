#pragma once
#include <Engine/Math/Transform.hpp>
#include <OpenGL.hpp>
#include <Engine/Camera.hpp>
#include <Engine/GameObject.hpp>

namespace Engine {
    class Player {
    private:
        GameObject* playerGameObject;

    public:
        Math::Transform transform;

        Player();
        void handleKeyboardInput(GLFWwindow * window, Camera* camera, float deltaTime);
        void setPlayerGameObject(GameObject* playerGameObject);
    };
}