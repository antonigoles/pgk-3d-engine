#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <iostream>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Engine/VolumetricParticles.hpp>
#include <Engine/Math/Math.hpp>
#include <Engine/GameObjectCluster.hpp>
#include <Engine/Colisions.hpp>
#include <Engine/SimplePhysics.hpp>

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
            glm::radians(this->camera[0]->getFOV() / 2.0f), 
            ((float)this->v_width) / ((float)this->v_height), 0.1f, 500.0f
        );
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && releaseStopButton) {
            stop = !stop;
            releaseStopButton = false;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            releaseStopButton = true;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            this->cursorSetFreeMode(window);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            this->cursorSetFocusMode(window);
        }

        this->camera[0]->handleMouseInput(window, deltaTime);
        this->player->handleKeyboardInput(window, this->camera[0], deltaTime);
        this->camera[0]->syncCameraAndTarget(player->transform);

        this->skybox->transform.setPosition(
            this->camera[0]->transform.getPosition()
        );
        
        float i = 0;
        for (auto shaderIdToObjectsPair : shaderRepository.shaderToObjectIDsMap) {
            EngineID shaderID = shaderIdToObjectsPair.first;
            shaderRepository.useShaderWithDataByID(shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[0]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[0]->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            for (EngineID engineID : shaderIdToObjectsPair.second) {
                i++;
                GameObject* gameObject = gameObjectRepository.getGameObject(engineID);
                if (!gameObject->isVisible()) continue;
                if(gameObject->hasParentCluster()) continue;

                gameObject->callUpdateFunctions(deltaTime);


                EngineID meshID = meshRepository.getMeshIDByGameObject(gameObject);
                shaderRepository.setUniformMat4("model", gameObject->transform.getModelMatrix());

                EngineID meshVAO = meshRepository.getMeshVAO(meshID);
                glBindVertexArray(meshVAO);
                glDrawArrays(GL_TRIANGLES, 0, meshRepository.getMeshSize(meshID));
            }
        }

        // Draw clusters
        for (auto gameObjectCluster : gameObjectClusterRepository.getClusters()) {
            shaderRepository.useShaderWithDataByID(gameObjectCluster->getShader(), {}, {});

            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[0]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[0]->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            glBindVertexArray(gameObjectCluster->getMeshVAO());
            for (auto object : gameObjectCluster->gameObjects) object->callUpdateFunctions(deltaTime);
            gameObjectCluster->updateAndBindSSBO();
            glDrawArraysInstanced(GL_TRIANGLES, 0, gameObjectCluster->getMeshSize(), gameObjectCluster->gameObjects.size());
        }

        // render particles
        auto sphericalGenerators = volumetricParticleGeneratorRepository.getAllSphericalGenerators();
        for (auto generator : sphericalGenerators) {
            // draw particle
            // 1. Set shader
            shaderRepository.useShaderWithDataByID(generator->shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[0]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[0]->transform.getPosition());

            // 2. load mesh and draw all particles
            // TODO: Maybe rewrite this with instancing in mind, might be simple
            EngineID meshVAO = meshRepository.getMeshVAO(generator->particleMeshId);
            glBindVertexArray(meshVAO);
            
            if (!volumetricParticleGeneratorRepository.stepSphericalGenerator(generator->generatorID, deltaTime)) {
                volumetricParticleGeneratorRepository.deleteSphericalGenerator(generator->generatorID);
            };
            glDrawArraysInstanced(GL_TRIANGLES, 0, meshRepository.getMeshSize(generator->particleMeshId), generator->particleCount - generator->killedParticles);
        }

        singleOBBDynamicMultiSphericalColiderColisionService.updateStructure();
        simplePhysics.progressEngine(deltaTime);
    };

    void Window::renderOnly(int cameraIndex, GLFWwindow * window) {
        glm::mat4 projection = glm::perspective(
            glm::radians(this->camera[cameraIndex]->getFOV() / 2.0f), 
            ((float)this->v_width) / ((float)this->v_height), 0.1f, 500.0f
        );

        // update camera position somehow
        this->camera[cameraIndex]->transform.setRotation(
            Math::lookAtQuat(
                {0.0f, 0.0f, 0.0f},
                this->camera[0]->getForward(),
                {0.0f, 1.0f, 0.0f}
            )
        );

        this->camera[cameraIndex]->syncCameraAndTarget(player->transform);
        this->skybox->transform.setPosition(
            this->camera[cameraIndex]->transform.getPosition()
        );
        this->player->playerGameObject->hide();
        float i = 0;
        for (auto shaderIdToObjectsPair : shaderRepository.shaderToObjectIDsMap) {
            EngineID shaderID = shaderIdToObjectsPair.first;
            shaderRepository.useShaderWithDataByID(shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[cameraIndex]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[cameraIndex]->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            for (EngineID engineID : shaderIdToObjectsPair.second) {
                i++;
                GameObject* gameObject = gameObjectRepository.getGameObject(engineID);
                if (!gameObject->isVisible()) continue;

                EngineID meshID = meshRepository.getMeshIDByGameObject(gameObject);
                shaderRepository.setUniformMat4("model", gameObject->transform.getModelMatrix());

                EngineID meshVAO = meshRepository.getMeshVAO(meshID);
                glBindVertexArray(meshVAO);
                glDrawArrays(GL_TRIANGLES, 0, meshRepository.getMeshSize(meshID));
            }
        }
        this->player->playerGameObject->show();

        // Draw clusters
        for (auto gameObjectCluster : gameObjectClusterRepository.getClusters()) {
            shaderRepository.useShaderWithDataByID(gameObjectCluster->getShader(), {}, {});

            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[0]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[0]->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            glBindVertexArray(gameObjectCluster->getMeshVAO());
            gameObjectCluster->updateAndBindSSBO();
            glDrawArraysInstanced(GL_TRIANGLES, 0, gameObjectCluster->getMeshSize(), gameObjectCluster->gameObjects.size());
        }

        // render particles
        auto sphericalGenerators = volumetricParticleGeneratorRepository.getAllSphericalGenerators();
        for (auto generator : sphericalGenerators) {
            shaderRepository.useShaderWithDataByID(generator->shaderID, {}, {});
            shaderRepository.setUniformMat4("projection", projection);
            shaderRepository.setUniformMat4("view", this->camera[cameraIndex]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[cameraIndex]->transform.getPosition());
            EngineID meshVAO = meshRepository.getMeshVAO(generator->particleMeshId);
            glBindVertexArray(meshVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, meshRepository.getMeshSize(generator->particleMeshId), generator->particleCount - generator->killedParticles);
        }
    };

    void Window::setCamera1(Camera *camera) {
        this->camera[0] = camera;
    };

    void Window::setCamera2(Camera *camera) {
        this->camera[1] = camera;
    };

    void Window::setPlayer(Player *player) {
        this->player = player;
    };

    void Window::setSkybox(GameObject *skybox) {
        this->skybox = skybox;
    }


    void Window::cursorSetFocusMode(GLFWwindow * window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    };

    void Window::cursorSetFreeMode(GLFWwindow * window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    };
};