#pragma once
#include <OpenGL.hpp>
#include <vector>
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>

namespace Engine
{
    class DynamicColider {
    public:
        GameObject *owner;
        glm::vec3 p1;
        glm::vec3 p2;
    };

    class StaticColider {
    public:
        GameObject *owner;
        glm::vec3 position;
        float radius;
    };

    class SingleOBBDynamicMultiSphericalColiderColisionService {
    private:
        DynamicColider dynamic;
        std::vector<StaticColider> sortedStructure;
    public:
        SingleOBBDynamicMultiSphericalColiderColisionService();

        void addStatic(GameObject *ref, float radius);
        void setDynamic(GameObject *ref, glm::vec3 p1, glm::vec3 p2);

        std::vector<StaticColider*> getColisions();
        void updateStructure();
    };

    extern SingleOBBDynamicMultiSphericalColiderColisionService singleOBBDynamicMultiSphericalColiderColisionService;
}