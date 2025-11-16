#pragma once
#include <Engine/GameObject.hpp>

namespace Engine {
    class AsteroidExplosionService {
    EngineID meshID;
    EngineID shaderID;

    public:
        AsteroidExplosionService();

        void setExplosionParticleMesh(EngineID meshID);
        void setExplosionParticleShader(EngineID shaderID);

        void explodeAsteroid(GameObject *asteroid, EngineID asteroidColiderID);
    };

    extern AsteroidExplosionService asteroidExplosionService;
}