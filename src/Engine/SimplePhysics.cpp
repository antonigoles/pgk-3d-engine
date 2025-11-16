#include <unordered_set>
#include <Engine/Math/Transform.hpp>
#include <Engine/SimplePhysics.hpp>
#include <iostream>

namespace Engine
{
    SimplePhysics::SimplePhysics() {};

    void SimplePhysics::progressEngine(float deltaTime) {
        int ptr = 0;
        while (ptr < this->effects.size()) {
            this->effects[ptr].timeLeft -= deltaTime;
            this->effects[ptr].timeLeft = std::max(0.0f, this->effects[ptr].timeLeft);
            glm::vec3 m_vel = this->effects[ptr].velocity * (this->effects[ptr].timeLeft/this->effects[ptr].time);
            this->effects[ptr].transform->setPosition(
                this->effects[ptr].transform->getPosition() + m_vel * deltaTime
            );
            if (this->effects[ptr].timeLeft <= 0.0f) {
                std::swap(this->effects[ptr], this->effects[this->effects.size()-1]);
                this->effects.pop_back();
            } else {
                ptr++;
            }
        }
    };

    void SimplePhysics::punch(Math::Transform *transform, float time, glm::vec3 velocity) {
        this->effects.push_back(SimplePhysicsEngineEffect{
            transform,
            time,
            time,
            velocity
        });
    };


    SimplePhysics simplePhysics = SimplePhysics();
};