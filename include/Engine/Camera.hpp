#pragma once
#include <Engine/Math/Transform.hpp>
#include <OpenGL.hpp>

namespace Engine
{
    class Camera {
    private:
        float FOV;
    
    public:
        Math::Transform transform;
        
        Camera(float FOV);
        glm::mat4 getViewMatrix();
        float getFOV();

        void handleMouseInput(GLFWwindow * window, float deltaTime);
    };
};
