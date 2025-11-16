#pragma once
#include <OpenGL.hpp>
#include <vector>
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>
#include <unordered_set>

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
        EngineID id;
        GameObject *owner;
        glm::vec3 position;
        float radius;
    };

    class SingleOBBDynamicMultiSphericalColiderColisionService {
    private:
        DynamicColider dynamic;
        std::unordered_map<EngineID, StaticColider> coliderMap;

        EngineID nextID;

        EngineID getNextID();
    public:
        SingleOBBDynamicMultiSphericalColiderColisionService();

        void addStatic(GameObject *ref, float radius);
        void removeStatic(EngineID coliderID);
        void setDynamic(GameObject *ref, glm::vec3 p1, glm::vec3 p2);

        std::vector<StaticColider*> getColisions();
        void updateStructure();
    };

    extern SingleOBBDynamicMultiSphericalColiderColisionService singleOBBDynamicMultiSphericalColiderColisionService;
}