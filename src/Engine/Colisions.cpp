#include <OpenGL.hpp>
#include <vector>
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Colisions.hpp>

namespace Engine
{
    SingleOBBDynamicMultiSphericalColiderColisionService::SingleOBBDynamicMultiSphericalColiderColisionService() {
    };

    void SingleOBBDynamicMultiSphericalColiderColisionService::addStatic(GameObject *ref, float radius) {
        this->sortedStructure.push_back(StaticColider{ref, ref->transform.getPosition(), radius});
    };

    void SingleOBBDynamicMultiSphericalColiderColisionService::setDynamic(GameObject *ref, glm::vec3 p1, glm::vec3 p2) {
        this->dynamic = DynamicColider{ref, p1, p2};
    };

    std::vector<StaticColider*> SingleOBBDynamicMultiSphericalColiderColisionService::getColisions() {
        std::vector<StaticColider*> result;

        glm::quat rot = this->dynamic.owner->transform.getRotation();
        float scale = this->dynamic.owner->transform.getScale();
        glm::vec3 p1 = this->dynamic.p1;
        glm::vec3 p2 = this->dynamic.p2;

        glm::vec3 obbCenterLocal = (p1 + p2) * 0.5f;
        glm::vec3 obbCenterWorld = this->dynamic.owner->transform.getPosition() + rot * (obbCenterLocal * scale);
        glm::vec3 obbHalfSize = (p2 - p1) * 0.5f * scale;
        glm::mat3 rotMat = glm::mat3_cast(rot);
        
        for (auto staticColider : this->sortedStructure) {
            auto sphereCenter = staticColider.position;
            auto sphereRadius = staticColider.radius;

            glm::vec3 d = sphereCenter - obbCenterWorld;
            glm::vec3 localCenter;
            localCenter.x = glm::dot(d, rotMat[0]);
            localCenter.y = glm::dot(d, rotMat[1]);
            localCenter.z = glm::dot(d, rotMat[2]);

            glm::vec3 closestPoint = glm::clamp(localCenter, -obbHalfSize, obbHalfSize);
            glm::vec3 diff = localCenter - closestPoint;
            float dist2 = glm::dot(diff, diff);

            if (dist2 <= sphereRadius * sphereRadius) {
                result.push_back(&staticColider);
            };
        }

        return result;
    };

    void SingleOBBDynamicMultiSphericalColiderColisionService::updateStructure() {
        for (auto staticColider : this->sortedStructure) {
            staticColider.position = staticColider.owner->transform.getPosition();
        }
    };

    SingleOBBDynamicMultiSphericalColiderColisionService singleOBBDynamicMultiSphericalColiderColisionService = SingleOBBDynamicMultiSphericalColiderColisionService();
}