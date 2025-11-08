#pragma once
#include <OpenGL.hpp>
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <unordered_map>

namespace Engine {

    class VolumetricParticle {
    public:
        glm::vec3 velocity;
        Math::Transform transform;
    };
    
    class SphericalVolumetricParticleGenerator {
    public:
        EngineID generatorID;
        glm::vec3 position;
        glm::vec3 actingForce;
        float radius;

        float particleSpeed;
        float speedVariance;
        float particleSize;
        float sizeVariance;
        float rotationVariance;

        int particleCount;
        std::vector<VolumetricParticle*> particlePool;

        EngineID particleMeshId;
        EngineID shaderID;
    };
    
    class VolumetricParticleGeneratorRepository {
    private:
        std::unordered_map<EngineID, SphericalVolumetricParticleGenerator*> sphericalSourceMap;

        EngineID lastID;

        EngineID getNextEngineID();

        void restartSphericalGeneratorParticle(
            SphericalVolumetricParticleGenerator* parentGenerator,
            VolumetricParticle* particle
        );
    public:
        VolumetricParticleGeneratorRepository();

        void createSphericalParticleSource(
            glm::vec3 position,
            glm::vec3 actingForce,
            float radius,
            float particleSpeed,
            float speedVariance,
            float particleSize,
            float sizeVariance,
            float rotationVariance,
            float particleCount,
            EngineID particleMeshId,
            EngineID shaderID
        );

        std::vector<SphericalVolumetricParticleGenerator*> getAllSphericalGenerators();
        void stepSphericalGenerator(EngineID generatorID, float deltaTime);

        // void createDirectionalParticleSource(
        //     glm::vec3 position,
        // );
    };

    extern VolumetricParticleGeneratorRepository volumetricParticleGeneratorRepository;
}