#pragma once
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>
#include <unordered_map>

namespace Engine 
{
    class GameObjectRepository {
    private:
        std::unordered_map<EngineID, GameObject*> gameObjectMap;

        EngineID gameObjectCounter = 0;

        EngineID getNextGameObjectIndex();
    public:
        GameObjectRepository();

        GameObject* getGameObject(EngineID engineID);

        std::pair<GameObject*, EngineID> createGameObject(EngineID mesh, EngineID shaderProgram);
    };

    extern GameObjectRepository gameObjectRepository;
};