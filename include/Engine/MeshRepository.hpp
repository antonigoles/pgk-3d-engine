#pragma once
#include <unordered_map>
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>
#include <string>

namespace Engine 
{
    class MeshRepository {
    private:
        std::unordered_map<EngineID, Mesh> meshMap;
        std::unordered_map<EngineID, unsigned int> VAOMap;
        std::unordered_map<GameObject*, EngineID> objectMap;

        unsigned int meshCounter = 0;

        EngineID getNextMeshIndex();
    public:
        std::unordered_map<EngineID, std::vector<GameObject*>> meshToGameObjects;

        MeshRepository();
        
        EngineID loadMeshFromFile(const std::string &path);
        EngineID loadMesh(const Mesh mesh);
        EngineID loadMeshWithNormals(const Mesh nmesh);
        
        unsigned int getMeshVAO(EngineID meshId); 
        
        size_t getMeshSize(EngineID meshId);

        EngineID getMeshIDByGameObject(GameObject *gameObject);

        void assignMesh(GameObject* gameObject, EngineID meshId);
    };

    extern MeshRepository meshRepository;
};

