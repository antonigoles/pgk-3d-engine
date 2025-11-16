#pragma once
#include <vector>
#include <Engine/Math/Transform.hpp>
#include <Engine/GameObject.hpp>

namespace Engine
{
    struct SimplePhysicsEngineEffect {
    public:
        Math::Transform *transform;
        float timeLeft;
        float time; 
        glm::vec3 velocity;
    };

    class SimplePhysics {
    private:
        std::vector<SimplePhysicsEngineEffect> effects;
    
    public:
        SimplePhysics();

        void progressEngine(float deltaTime);

        void punch(Math::Transform *transform, float time, glm::vec3 velocity);
    };

    extern SimplePhysics simplePhysics;
};