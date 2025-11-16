#pragma once
#include <OpenGL.hpp>
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <unordered_map>

namespace Engine {

    class VolumetricParticle {
    public:
        glm::vec3 velocity;
        glm::vec3 actingForce;
        glm::vec3 startingPosition;
        Math::Transform transform;
        int lifes;
        bool isPaused;
    };
    struct b_ParticleDynamic {
        glm::vec3 position;
        float scale;
    };
    
    class SphericalVolumetricParticleGenerator {
    public:
        EngineID generatorID;
        glm::vec3 position;
        glm::vec3 actingForce;
        float radius;
        int maxParticleLifes;

        bool isPaused;
        int killedParticles;

        float particleSpeed;
        float speedVariance;
        float particleSize;
        float sizeVariance;
        float rotationVariance;

        int particleCount;
        std::vector<VolumetricParticle*> particlePool;

        EngineID particleMeshId;
        EngineID shaderID;

        GLuint staticSSBO;
        GLuint dynamicSSBO;
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

        std::pair<SphericalVolumetricParticleGenerator*, EngineID> createSphericalParticleSource(
            glm::vec3 position,
            glm::vec3 actingForce,
            int maxParticleLifes,
            float radius,
            float particleSpeed,
            float speedVariance,
            float particleSize,
            float sizeVariance,
            float rotationVariance,
            float particleCount,
            EngineID particleMeshId,
            EngineID shaderID,
            bool hardPaused
        );

        std::vector<SphericalVolumetricParticleGenerator*> getAllSphericalGenerators();
        bool stepSphericalGenerator(EngineID generatorID, float deltaTime);
        void deleteSphericalGenerator(EngineID generatorID);
        void resetGenerator(EngineID generatorID);

        // void createDirectionalParticleSource(
        //     glm::vec3 position,
        // );
    };

    extern VolumetricParticleGeneratorRepository volumetricParticleGeneratorRepository;
}