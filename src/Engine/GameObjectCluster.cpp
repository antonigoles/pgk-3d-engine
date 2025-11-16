#include <Engine/GameObjectCluster.hpp>
#include <Engine/MeshRepository.hpp>

namespace Engine 
{
    // ************** GameObjectCluster **************

    // GLuint meshVAO;
    // GLuint SSBO;
    // int meshSize;
    // std::vector<GameObject*> gameObjects;
    
    GameObjectCluster::GameObjectCluster() {}

    void GameObjectCluster::setMesh(EngineID meshID) {
        this->meshVAO = meshRepository.getMeshVAO(meshID);
        this->meshSize = meshRepository.getMeshSize(meshID);
    };

    void GameObjectCluster::updateAndBindSSBO() {
        std::vector<SSBO_Transform> updatedData;
        
        for (auto obj : this->gameObjects) {
            updatedData.push_back({obj->transform.getModelMatrix()});
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->SSBO);
        glBufferSubData(
            GL_SHADER_STORAGE_BUFFER, 
            0, 
            sizeof(SSBO_Transform) * updatedData.size(), 
            updatedData.data()
        );
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->SSBO);
    };

    void GameObjectCluster::initiateSSBO() {
        GLuint ssbo;
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(
            GL_SHADER_STORAGE_BUFFER, 
            sizeof(SSBO_Transform) * this->gameObjects.size(), 
            nullptr, 
            GL_DYNAMIC_DRAW
        );
        this->SSBO = ssbo;
    };

    void GameObjectCluster::setShader(EngineID shaderID) {
        this->shaderId = shaderID;
    };

    EngineID GameObjectCluster::getShader() {
        return this->shaderId;
    };
    
    GLuint GameObjectCluster::getMeshVAO() {
        return this->meshVAO;
    };

    unsigned int GameObjectCluster::getMeshSize() {
        return this->meshSize;
    };

    void GameObjectCluster::addObject(GameObject *gameObject) {
        this->gameObjects.push_back(gameObject);
    };


    // ************** GameObjectClusterRepository **************

    GameObjectClusterRepository::GameObjectClusterRepository() {
        this->lastId = 0;
    };

    EngineID GameObjectClusterRepository::getNextID() {
        return ++this->lastId;
    };

    GameObjectCluster* GameObjectClusterRepository::getCluster(const EngineID& clusterID) {
        return this->clusterMap[clusterID];
    };

    std::vector<GameObjectCluster*> GameObjectClusterRepository::getClusters() {
        std::vector<GameObjectCluster*> result;
        for (auto p : this->clusterMap) result.push_back(p.second);
        return result;
    };

    std::pair<GameObjectCluster*, EngineID> GameObjectClusterRepository::createCluster() {
        // TODO: Implement
        EngineID newId = this->getNextID();
        GameObjectCluster* cluster = new GameObjectCluster();
        this->clusterMap[newId] = cluster;
        return {cluster, newId};
    };

    GameObjectClusterRepository gameObjectClusterRepository = GameObjectClusterRepository();
}