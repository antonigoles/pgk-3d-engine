#pragma once
#include <Engine/Math/Transform.hpp>
#include <OpenGL.hpp>

namespace Engine
{
    class Camera {
    protected:
        float FOV;
    
    public:
        Math::Transform transform;
        
        Camera(float FOV);

        virtual glm::mat4 getViewMatrix();
        virtual float getFOV();

        virtual void handleMouseInput(GLFWwindow * window, float deltaTime);
        virtual void syncCameraAndTarget(Math::Transform& target);

        virtual glm::vec3 getForward();
        virtual glm::vec3 getUp();
        virtual glm::vec3 getRight();
    };
};
