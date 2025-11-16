#pragma once
#include <Engine/GameObject.hpp>
#include <OpenGL.hpp>

namespace Engine 
{
    struct SSBO_Transform {
        glm::mat4x4 model;
        // glm::vec3 position; 
        // glm::vec4 rotation;
        // float scale; 
        // and no padding!!!
    };

    class GameObjectCluster {
    private:
        GLuint meshVAO;
        GLuint SSBO;
        
        EngineID shaderId;

        int meshSize;
    public:
        GameObjectCluster();
        
        std::vector<GameObject*> gameObjects;
        void setMesh(EngineID meshID);
        void setShader(EngineID shaderID);

        void updateAndBindSSBO();
        void initiateSSBO();

        EngineID getShader();
        
        GLuint getMeshVAO();
        unsigned int getMeshSize();

        void addObject(GameObject *gameObject);
    };


    class GameObjectClusterRepository {
    private:
        std::unordered_map<EngineID, GameObjectCluster*> clusterMap;

        EngineID lastId;

        EngineID getNextID();
    public:
        GameObjectClusterRepository();

        GameObjectCluster* getCluster(const EngineID& clusterID);
        std::vector<GameObjectCluster*> getClusters();
        std::pair<GameObjectCluster*, EngineID> createCluster();
    };


    extern GameObjectClusterRepository gameObjectClusterRepository;
}