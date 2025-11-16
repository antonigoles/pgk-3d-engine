#pragma once
#include <OpenGL.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Player.hpp>

namespace Engine {
    enum WireframeRenderingMode {
        NONE = 0,
        MINOR_VIEWPORT = 1,
        MAJOR_VIEWPORT = 2,
        BOTH_VIEWPORTS = 4
    };

    class Window {
    private:
        Camera *camera[2];
        Player *player;
        GameObject *skybox;

        int v_width, v_height;
        WireframeRenderingMode wireframeRenderingMode;

    public:
        Window(int v_width, int v_height);
        void setViewportDimensions(int v_width, int v_height);
        void step(GLFWwindow * window, float deltaTime);
        void renderOnly(int cameraIndex, GLFWwindow * window);
        void setCamera1(Camera *camera);
        void setCamera2(Camera *camera);
        void setPlayer(Player *player);
        void setSkybox(GameObject *skybox);

        void cursorSetFocusMode(GLFWwindow * window);
        void cursorSetFreeMode(GLFWwindow * window);

        void setNextWireframeRenderingMode();
        void setPreviousWireframeRenderingMode();

        void setPolygonModeForMinorViewport();
        void setPolygonModeForMajorViewport();

        bool inputClickedOnce(GLFWwindow *window, int key);
    };
};