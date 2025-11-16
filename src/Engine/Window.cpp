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
#include <Engine/GameEndService.hpp>

namespace Engine {
    Window::Window(int v_width, int v_height) : v_width(v_width), v_height(v_height) {
        this->wireframeRenderingMode = NONE;
    };

    void Window::setNextWireframeRenderingMode() {
        switch (this->wireframeRenderingMode)
        {
            case WireframeRenderingMode::NONE:
                this->wireframeRenderingMode = WireframeRenderingMode::MINOR_VIEWPORT;
                break;
            case WireframeRenderingMode::MINOR_VIEWPORT:
                this->wireframeRenderingMode = WireframeRenderingMode::MAJOR_VIEWPORT;
                break;
            case WireframeRenderingMode::MAJOR_VIEWPORT:
                this->wireframeRenderingMode = WireframeRenderingMode::BOTH_VIEWPORTS;
                break;
            case WireframeRenderingMode::BOTH_VIEWPORTS:
                this->wireframeRenderingMode = WireframeRenderingMode::NONE;
                break;
        }
    };

    void Window::setPreviousWireframeRenderingMode() {
        switch (this->wireframeRenderingMode)
        {
            case WireframeRenderingMode::MINOR_VIEWPORT:
                this->wireframeRenderingMode = WireframeRenderingMode::NONE;
                break;
            case WireframeRenderingMode::MAJOR_VIEWPORT:
                this->wireframeRenderingMode = WireframeRenderingMode::MINOR_VIEWPORT;
                break;
            case WireframeRenderingMode::BOTH_VIEWPORTS:
                this->wireframeRenderingMode = WireframeRenderingMode::MAJOR_VIEWPORT;
                break;
            case WireframeRenderingMode::NONE:
                this->wireframeRenderingMode = WireframeRenderingMode::BOTH_VIEWPORTS;
                break;
        }
    };

    void Window::setPolygonModeForMinorViewport() {
        switch (this->wireframeRenderingMode)
        {
            case WireframeRenderingMode::MINOR_VIEWPORT:
            case WireframeRenderingMode::BOTH_VIEWPORTS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case WireframeRenderingMode::MAJOR_VIEWPORT:
            case WireframeRenderingMode::NONE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
                break;
        }
    };

    void Window::setPolygonModeForMajorViewport() {
        switch (this->wireframeRenderingMode)
        {
            case WireframeRenderingMode::MAJOR_VIEWPORT:
            case WireframeRenderingMode::BOTH_VIEWPORTS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case WireframeRenderingMode::MINOR_VIEWPORT:
            case WireframeRenderingMode::NONE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
                break;
        }
    };
    
    void Window::setViewportDimensions(int v_width, int v_height) {
        this->v_width = v_width;
        this->v_height = v_height;
    };

    bool Window::inputClickedOnce(GLFWwindow *window, int key) {
        static std::unordered_map<int, int> keyPreviousFrameState;
        auto state = glfwGetKey(window, key);
        
        if (!keyPreviousFrameState.contains(key) || keyPreviousFrameState[key] != GLFW_PRESS) {
            if (state == GLFW_PRESS) {
                keyPreviousFrameState[key] = state;
                return true;
            }
        }

        keyPreviousFrameState[key] = state;
        return false;
    };

    void Window::step(GLFWwindow * window, float deltaTime) {
        glm::mat4 projection = glm::perspective(
            glm::radians(this->camera[0]->getFOV() / 2.0f), 
            ((float)this->v_width) / ((float)this->v_height), 0.1f, 500.0f
        );
        
        if (this->inputClickedOnce(window, GLFW_KEY_UP)) {
            this->setNextWireframeRenderingMode();
        }

        if (this->inputClickedOnce(window, GLFW_KEY_DOWN)) {
            this->setPreviousWireframeRenderingMode();
        }

        if (this->inputClickedOnce(window, GLFW_KEY_ESCAPE)) {
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
        
        this->setPolygonModeForMajorViewport();

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
                if(!gameObject->isEnabled()) continue;

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
            for (auto object : gameObjectCluster->gameObjects) {
                if (object->isEnabled())
                    object->callUpdateFunctions(deltaTime);
            }
            unsigned int countToDraw = gameObjectCluster->updateAndBindSSBO();
            glDrawArraysInstanced(GL_TRIANGLES, 0, gameObjectCluster->getMeshSize(), countToDraw);
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
                continue;
            };
            glDrawArraysInstanced(GL_TRIANGLES, 0, meshRepository.getMeshSize(generator->particleMeshId), generator->particleCount - generator->killedParticles);
        }

        singleOBBDynamicMultiSphericalColiderColisionService.updateStructure();
        simplePhysics.progressEngine(deltaTime);

        gameEndService.progressGameEndAnimation(deltaTime);
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
                player->playerGameObject->transform.getRotation() * glm::vec3{0.0f, 0.0f, -1.0f},
                {0.0f, 1.0f, 0.0f}
            )
        );

        this->camera[cameraIndex]->syncCameraAndTarget(player->playerGameObject->transform);
        this->skybox->transform.setPosition(
            this->camera[cameraIndex]->transform.getPosition()
        );
        
        this->setPolygonModeForMinorViewport();

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
                if (!gameObject->isEnabled()) continue;

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
            shaderRepository.setUniformMat4("view", this->camera[cameraIndex]->getViewMatrix());
            shaderRepository.setUniformVec3("viewPos", this->camera[cameraIndex]->transform.getPosition());
            shaderRepository.setUniformVec3("lightPos", glm::vec3(0.0f, 0.0f, -150.0f));

            glBindVertexArray(gameObjectCluster->getMeshVAO());
            unsigned int countToDraw = gameObjectCluster->updateAndBindSSBO();
            glDrawArraysInstanced(GL_TRIANGLES, 0, gameObjectCluster->getMeshSize(), countToDraw);
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