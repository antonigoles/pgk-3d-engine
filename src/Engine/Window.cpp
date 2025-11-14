#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <iostream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Engine/VolumetricParticles.hpp>

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
            ((float)this->v_width) / ((float)this->v_height), 0.1f, 500.0f
        );
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && releaseStopButton) {
            stop = !stop;
            releaseStopButton = false;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            releaseStopButton = true;
        }

        this->camera->handleMouseInput(window, deltaTime);
        this->player->handleKeyboardInput(window, this->camera, deltaTime);
        this->camera->syncCameraAndTarget(player->transform);

        this->skybox->transform.setPosition(
            this->camera->transform.getPosition()
        );
        
        float i = 0;
        for (auto shaderIdToObjectsPair : shaderRepository.shaderToObjectIDsMap) {
            EngineID shaderID = shaderIdToObjectsPair.first;
            shaderRepository.useShaderWithDataByID(shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            for (EngineID engineID : shaderIdToObjectsPair.second) {
                i++;
                GameObject* gameObject = gameObjectRepository.getGameObject(engineID);
                if (!gameObject->isVisible()) continue;

                gameObject->callUpdateFunctions(deltaTime);

                EngineID meshID = meshRepository.getMeshIDByGameObject(gameObject);
                shaderRepository.setUniformMat4("model", gameObject->transform.getModelMatrix());

                EngineID meshVAO = meshRepository.getMeshVAO(meshID);
                glBindVertexArray(meshVAO);
                glDrawArrays(GL_TRIANGLES, 0, meshRepository.getMeshSize(meshID));
            }
        }

        // render particles
        auto sphericalGenerators = volumetricParticleGeneratorRepository.getAllSphericalGenerators();
        for (auto generator : sphericalGenerators) {
            // draw particle
            // 1. Set shader
            shaderRepository.useShaderWithDataByID(generator->shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera->transform.getPosition());

            // 2. load mesh and draw all particles
            // TODO: Maybe rewrite this with instancing in mind, might be simple
            EngineID meshVAO = meshRepository.getMeshVAO(generator->particleMeshId);
            glBindVertexArray(meshVAO);
            
            volumetricParticleGeneratorRepository.stepSphericalGenerator(generator->generatorID, deltaTime);
            glDrawArraysInstanced(GL_TRIANGLES, 0, meshRepository.getMeshSize(generator->particleMeshId), generator->particleCount);
        }
    };

    void Window::setCamera(Camera *camera) {
        this->camera = camera;
    };

    void Window::setPlayer(Player *player) {
        this->player = player;
    };

    void Window::setSkybox(GameObject *skybox) {
        this->skybox = skybox;
    }
};