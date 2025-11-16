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
        
        EngineID loadMeshFromFileWithPreprocessing(const std::string &path, glm::quat rotation);
        EngineID loadMeshFromFile(const std::string &path);

        EngineID loadMeshFromOBJFileWithPreprocessing(const std::string &path, glm::quat rotation);
        EngineID loadMeshFromOBJFile(const std::string &path);

        EngineID loadMesh(const Mesh mesh);
        EngineID loadMeshWithNormals(const Mesh nmesh);
        
        unsigned int getMeshVAO(EngineID meshId); 
        
        size_t getMeshSize(EngineID meshId);

        EngineID getMeshIDByGameObject(GameObject *gameObject);

        void assignMesh(GameObject* gameObject, EngineID meshId);

        std::pair<glm::vec3, glm::vec3> getMeshCuboid(EngineID mesh, float padding);
    };

    extern MeshRepository meshRepository;
};

