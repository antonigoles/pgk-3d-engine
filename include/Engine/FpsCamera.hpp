#pragma once
#include <Engine/Math/Transform.hpp>
#include <Engine/Camera.hpp>
#include <OpenGL.hpp>

namespace Engine
{
    class FpsCamera : public Camera {
    public:        
        FpsCamera(float FOV);

        glm::mat4 getViewMatrix() override;
        float getFOV() override;

        void handleMouseInput(GLFWwindow * window, float deltaTime) override;
        void syncCameraAndTarget(Math::Transform& target) override;

        glm::vec3 getForward() override;
        glm::vec3 getUp() override;
        glm::vec3 getRight() override;
    };
};
