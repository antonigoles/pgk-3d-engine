#include <iostream>
#include <Engine/GameObject.hpp>
#include <Engine/Math/Math.hpp>
#include <OpenGL.hpp>

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
        Engine::GameObject* gameObject = (Engine::GameObject*)data.sourcePointer;

        gameObject->transform.setRotation(
            glm::angleAxis(data.deltaTime, gameObject->transform.getRotation() * glm::vec3(0.0f, 0.0f, -1.0f)) *
            gameObject->transform.getRotation()
        );

        if (!gameObject->has_vec3("forward")) return;
        if (!gameObject->has_float("velocity")) return;

        glm::vec3 forward = gameObject->get_vec3("forward");
        float velocity = gameObject->get_float("velocity");

        gameObject->transform.setRotation(
            Engine::Math::rotateTowards(gameObject->transform.getRotation(), forward, data.deltaTime * velocity * 4.0f)
        );


        
    };
}