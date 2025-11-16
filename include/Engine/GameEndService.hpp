#pragma once
#include <Engine/GameObject.hpp>

namespace Engine {
    class GameEndActors {
    public:
        GameObject *playerObject;
        GameObject *lightObject;
    };

    class GameEndService {
    private:
        bool gameEndStarted;
        GameEndActors gameEndActorsData;

    public:
        GameEndService();

        void startGameEnd();

        void updateGameEndActors(GameEndActors gameEndActorsData);

        void progressGameEndAnimation(float deltaTime);
    };

    extern GameEndService gameEndService;
}