#include <Engine/VolumetricParticles.hpp>
#include <Engine/Math/Math.hpp>

namespace Engine {
    
    VolumetricParticleGeneratorRepository::VolumetricParticleGeneratorRepository() {
        lastID = 0;
    };

    EngineID VolumetricParticleGeneratorRepository::getNextEngineID() {
        return lastID++;
    }

    void VolumetricParticleGeneratorRepository::createSphericalParticleSource(
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
    ) {
        EngineID id = this->getNextEngineID();

        SphericalVolumetricParticleGenerator* svpg = new SphericalVolumetricParticleGenerator();
        svpg->generatorID = id;
        svpg->position = position;
        svpg->radius = radius;
        svpg->actingForce = actingForce;
        
        svpg->particleSpeed = particleSpeed;
        svpg->speedVariance = speedVariance;
        svpg->particleSize = particleSize;
        svpg->sizeVariance = sizeVariance;
        svpg->rotationVariance = rotationVariance;

        svpg->particleCount = particleCount;
        svpg->particleMeshId = particleMeshId;
        svpg->shaderID = shaderID;
        
        std::vector<VolumetricParticle*> startingParticleVector;
        for (int i = 0; i<particleCount; i++) {
            VolumetricParticle* vp = new VolumetricParticle();
            this->restartSphericalGeneratorParticle(svpg, vp);
            startingParticleVector.push_back(vp);
        }
        
        svpg->particlePool = startingParticleVector;
    
        this->sphericalSourceMap[id] = svpg;
    };


    std::vector<SphericalVolumetricParticleGenerator*> VolumetricParticleGeneratorRepository::getAllSphericalGenerators() {
        std::vector<SphericalVolumetricParticleGenerator*> generators;
        for (auto p : this->sphericalSourceMap) {
            generators.push_back(p.second); 
        }
        return generators;
    };

    void VolumetricParticleGeneratorRepository::stepSphericalGenerator(EngineID generatorID, float deltaTime) {
        SphericalVolumetricParticleGenerator* generator = this->sphericalSourceMap[generatorID];
        for (VolumetricParticle* particle : generator->particlePool) {
            particle->velocity += generator->actingForce * deltaTime;
            particle->transform.setPosition(
               particle->transform.getPosition() + 
               particle->velocity * deltaTime 
            );
            if (
                glm::length2(particle->transform.getPosition() - generator->position) >= 
                generator->radius * generator->radius 
            ) {
                this->restartSphericalGeneratorParticle(generator, particle);
            }
        }
    };

    void VolumetricParticleGeneratorRepository::restartSphericalGeneratorParticle(
        SphericalVolumetricParticleGenerator* parentGenerator,
        VolumetricParticle* particle
    ) {

        // 1. Generate random velocity direction
        particle->velocity = glm::vec3{ 
            Math::getRandom(-1.0f, 1.0f),
            Math::getRandom(-1.0f, 1.0f),
            Math::getRandom(-1.0f, 1.0f)
        };
        particle->velocity = glm::normalize(particle->velocity);

        // 2. reset position
        particle->transform.setPosition(parentGenerator->position);

        // 3. Randomize speed
        float speedVarianceFactor = Math::getRandom(
            -parentGenerator->speedVariance, 
            parentGenerator->speedVariance
        );
        particle->velocity = particle->velocity * parentGenerator->particleSpeed * (1.0f + speedVarianceFactor);

        // 4. Randomize size
        float sizeVarianceFactor = Math::getRandom(
            -parentGenerator->sizeVariance, 
            parentGenerator->sizeVariance
        );
        particle->transform.setScale(parentGenerator->particleSize * (1.0f + sizeVarianceFactor));

        // 5. Randomize rotation
        float rotationVarianceFactorX = Math::getRandom(
            -parentGenerator->rotationVariance, 
            parentGenerator->rotationVariance
        ) * 3.14;
        float rotationVarianceFactorY = Math::getRandom(
            -parentGenerator->rotationVariance, 
            parentGenerator->rotationVariance
        ) * 3.14;
        float rotationVarianceFactorZ = Math::getRandom(
            -parentGenerator->rotationVariance, 
            parentGenerator->rotationVariance
        ) * 3.14;

        particle->transform.setRotation(
            glm::quat({rotationVarianceFactorX, rotationVarianceFactorY, rotationVarianceFactorZ})
        );
    };

    VolumetricParticleGeneratorRepository volumetricParticleGeneratorRepository = VolumetricParticleGeneratorRepository();
}