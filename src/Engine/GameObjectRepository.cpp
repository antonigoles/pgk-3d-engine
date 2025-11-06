#include <Engine/GameObjectRepository.hpp>
#include <Engine/Misc/Types.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <Engine/GameObject.hpp>
#include <unordered_map>

namespace Engine 
{
    GameObjectRepository::GameObjectRepository() {};

    EngineID GameObjectRepository::getNextGameObjectIndex() {
        return this->gameObjectCounter++;
    };
    
    std::pair<GameObject*, EngineID> GameObjectRepository::createGameObject(
        EngineID mesh, 
        EngineID shaderProgram
    ) {
        EngineID id = this->getNextGameObjectIndex();
        GameObject* gameObject = new GameObject();
        
        this->gameObjectMap[id] = gameObject;

        Engine::shaderRepository.assignShaderIDToGameObjectID(shaderProgram, id);
        
        Engine::meshRepository.assignMesh(gameObject, mesh);

        return {gameObject, id};
    };

    GameObject* GameObjectRepository::getGameObject(EngineID engineID) {
        return this->gameObjectMap[engineID];
    };

    GameObjectRepository gameObjectRepository = GameObjectRepository();
};