#include <Engine/AsteroidExplosionService.hpp>
#include <Engine/GameObjectCluster.hpp>
#include <Engine/VolumetricParticles.hpp>
#include <Engine/Colisions.hpp>
#include <iostream>

namespace Engine {
    AsteroidExplosionService::AsteroidExplosionService() {

    };

    void AsteroidExplosionService::setExplosionParticleMesh(EngineID meshID) {
        this->meshID = meshID;
    };
    
    void AsteroidExplosionService::setExplosionParticleShader(EngineID shaderID) {
        this->shaderID = shaderID;
    };

    void AsteroidExplosionService::explodeAsteroid(GameObject *asteroid, EngineID asteroidColiderID) {
        if (asteroid == nullptr) {
            std::cout << "nullptr passed\n";
        } 
        if (!asteroid->hasParentCluster()) {
            std::cout << "This asteroid does not have a parent cluster!";
            return;
        }
        auto pos = asteroid->transform.getPosition();
        GameObjectCluster* parentCluster = (GameObjectCluster*)asteroid->getParentCluster();
        parentCluster->deleteObject(asteroid);
        singleOBBDynamicMultiSphericalColiderColisionService.removeStatic(asteroidColiderID);

        auto rocketParticlesGenerator = volumetricParticleGeneratorRepository.createSphericalParticleSource(
            pos, // position
            glm::vec3{0,0,0}, // acting force
            1,
            8.0f, // radius
            8.0f, // particleSpeed
            0.6f, // speedVariance
            2.0f, // particleSize
            0.4f, // sizeVariance
            1.0f, // rotationVariance
            16, // particle count
            this->meshID,
            this->shaderID,
            false // hard paused
        );
        delete asteroid;
    };

    AsteroidExplosionService asteroidExplosionService = AsteroidExplosionService();
}