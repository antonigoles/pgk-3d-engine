#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <iostream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine {
    Window::Window(int v_width, int v_height) : v_width(v_width), v_height(v_height) 
    {};

    void Window::setViewportDimensions(int v_width, int v_height) {
        this->v_width = v_width;
        this->v_height = v_height;
    };

    void Window::step(GLFWwindow * window, float deltaTime) {
        static bool releaseStopButton = true;
        static bool stop = false;
        glm::mat4 projection = glm::perspective(
            glm::radians(this->camera->getFOV() / 2.0f), 
            ((float)this->v_width) / ((float)this->v_height), 0.1f, 100.0f
        );
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && releaseStopButton) {
            stop = !stop;
            releaseStopButton = false;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            releaseStopButton = true;
        }

        this->camera->handleMouseInput(window, deltaTime);
        
        float i = 0;
        for (auto shaderIdToObjectsPair : shaderRepository.shaderToObjectIDsMap) {
            EngineID shaderID = shaderIdToObjectsPair.first;
            shaderRepository.useShaderWithDataByID(shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera->transform.getPosition());

            for (EngineID engineID : shaderIdToObjectsPair.second) {
                i++;
                GameObject* gameObject = gameObjectRepository.getGameObject(engineID);

                if (gameObject->label != "lightSource" && !stop) {
                    gameObject->transform.setRotation(
                        glm::angleAxis(deltaTime * i/10, glm::vec3(0.0f, 1.0f, 0.0f)) *
                        gameObject->transform.getRotation()
                    );

                    gameObject->transform.setPosition(
                        glm::vec3(
                            gameObject->transform.getPosition().x, 
                            sin(glfwGetTime() + i), 
                            gameObject->transform.getPosition().z
                        )
                    );
                }
                

                EngineID meshID = meshRepository.getMeshIDByGameObject(gameObject);
                shaderRepository.setUniformMat4("model", gameObject->transform.getModelMatrix());

                EngineID meshVAO = meshRepository.getMeshVAO(meshID);
                glBindVertexArray(meshVAO);
                glDrawArrays(GL_TRIANGLES, 0, meshRepository.getMeshSize(meshID));
            }
        }
    };

    void Window::setCamera(Camera *camera) {
        this->camera = camera;
    };
};