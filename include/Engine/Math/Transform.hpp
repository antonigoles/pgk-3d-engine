#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine::Math
{
    class Transform {
    private:
        glm::vec3 position;
        glm::quat rotation;
        float scale;
        glm::mat4 modelMatrix;

        bool pendingModelMatrixCalculation = false;

        void recalculateModelMatrix();
        void invalidateModelMatrix();
        bool isModelMatrixValid();
    public:
        Transform();

        glm::vec3 getPosition();
        glm::quat getRotation();
        float getScale();

        void setPosition(glm::vec3 position);
        void setRotation(glm::quat rotation);
        void setScale(float scale);

        glm::mat4 getModelMatrix();
    };
};
