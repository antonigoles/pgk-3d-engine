#include <iostream>
#include <Engine/GameObject.hpp>
#include <Engine/Math/Math.hpp>
#include <OpenGL.hpp>
#include <Engine/VolumetricParticles.hpp>
#include <Engine/Colisions.hpp>
#include <Engine/SimplePhysics.hpp>
#include <Engine/Player.hpp>

namespace Game 
{
    void updateBall(Engine::UpdateFunctionData data) {
        Engine::GameObject* gameObject = (Engine::GameObject*)data.sourcePointer;

        if (!gameObject->has_float("script_init")) {
            gameObject->set_float("script_init", 1.0);
            gameObject->set_float("baseY", gameObject->transform.getPosition().y);
        }


        gameObject->transform.setRotation(
            glm::angleAxis(data.deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)) *
            gameObject->transform.getRotation()
        );

        gameObject->transform.setPosition(
            glm::vec3(
                gameObject->transform.getPosition().x, 
                gameObject->get_float("baseY") + sin(glfwGetTime() + gameObject->get_float("index")), 
                gameObject->transform.getPosition().z
            )
        );
    };

    void updatePlayerGameObject(Engine::UpdateFunctionData data) {
        static bool init = false;
        Engine::GameObject* gameObject = (Engine::GameObject*)data.sourcePointer;

        gameObject->transform.setRotation(
            glm::angleAxis(data.deltaTime, gameObject->transform.getRotation() * glm::vec3(0.0f, 0.0f, -1.0f)) *
            gameObject->transform.getRotation()
        );

        Engine::SphericalVolumetricParticleGenerator* particleGenerator = (Engine::SphericalVolumetricParticleGenerator*)gameObject->get_ref("particleGenerator");
        if (!init) {
            Engine::volumetricParticleGeneratorRepository.resetGenerator(particleGenerator->generatorID);
        }
        
        auto geForward = gameObject->transform.getRotation() * glm::vec3{0.0f, 0.0f, -1.0f};
        particleGenerator->actingForce = -10.0f * geForward;
        particleGenerator->position = gameObject->transform.getPosition() - 1.5f * geForward;

        if (!gameObject->has_vec3("forward")) return;
        if (!gameObject->has_float("velocity")) return;

        glm::vec3 forward = gameObject->get_vec3("forward");
        float velocity = gameObject->get_float("velocity");

        gameObject->transform.setRotation(
            Engine::Math::rotateTowards(gameObject->transform.getRotation(), forward, data.deltaTime * velocity * 4.0f)
        );
        
        if (velocity > 0.0) {
            particleGenerator->isPaused = false;
        } else {
            particleGenerator->isPaused = true;
        }

        auto colisions = Engine::singleOBBDynamicMultiSphericalColiderColisionService.getColisions();
        for (auto colision : colisions) {
            Engine::Player* player = (Engine::Player*)gameObject->get_ref("playerObject");
            glm::vec3 dir = glm::normalize(colision->position - gameObject->transform.getPosition());
            Engine::simplePhysics.punch(&player->transform, 1.0f, 2.0f * dir);
            break;
        }

        if (!init) init = true;
    };
}