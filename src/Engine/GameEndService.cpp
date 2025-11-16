#include <Engine/GameEndService.hpp>
#include <Engine/GameObjectRepository.hpp>
#include <Engine/VolumetricParticles.hpp>

namespace Engine {
    GameEndService::GameEndService() {
        this->gameEndStarted = false;
    };

    void GameEndService::startGameEnd() {
        this->gameEndStarted = true;
    };

    void GameEndService::updateGameEndActors(GameEndActors gameEndActorsData) {
        this->gameEndActorsData = gameEndActorsData;
    };

    void GameEndService::progressGameEndAnimation(float deltaTime) {
        static bool init = false;
        if (!this->gameEndStarted) return;
        if (!init) {
            for (auto& object : gameObjectRepository.gellAllGameObjects()) {
                object->disable();
            }
            this->gameEndActorsData.lightObject->enable();
            for (auto& gen : volumetricParticleGeneratorRepository.getAllSphericalGenerators()) {
                volumetricParticleGeneratorRepository.deleteSphericalGenerator(gen->generatorID);
            }
            init=true;
        }
        float scale = this->gameEndActorsData.lightObject->transform.getScale() + deltaTime * 100.0f;
        scale = scale > 100.0f ? 100.0f : scale; 
        this->gameEndActorsData.lightObject->transform.setScale(scale);
    };

    GameEndService gameEndService = GameEndService();
}