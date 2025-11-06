#pragma once
#include <OpenGL.hpp>
#include <Engine/Camera.hpp>

namespace Engine {
    class Window {
    private:
        Camera *camera;
        int v_width, v_height;

    public:
        Window(int v_width, int v_height);
        void setViewportDimensions(int v_width, int v_height);
        void step(GLFWwindow * window, float deltaTime);
        void setCamera(Camera *camera);
    };
};