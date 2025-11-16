#include <Engine/VolumetricParticles.hpp>
#include <Engine/Math/Math.hpp>
#include <OpenGL.hpp>

namespace Engine {
    
    VolumetricParticleGeneratorRepository::VolumetricParticleGeneratorRepository() {
        lastID = 0;
    };

    EngineID VolumetricParticleGeneratorRepository::getNextEngineID() {
        return lastID++;
    }

    std::pair<SphericalVolumetricParticleGenerator*, EngineID> VolumetricParticleGeneratorRepository::createSphericalParticleSource(
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
    ) {
        EngineID id = this->getNextEngineID();

        SphericalVolumetricParticleGenerator* svpg = new SphericalVolumetricParticleGenerator();
        svpg->generatorID = id;
        svpg->position = position;
        svpg->radius = radius;
        svpg->actingForce = actingForce;
        svpg->maxParticleLifes = maxParticleLifes;
        svpg->killedParticles = 0;
        
        svpg->particleSpeed = particleSpeed;
        svpg->speedVariance = speedVariance;
        svpg->particleSize = particleSize;
        svpg->sizeVariance = sizeVariance;
        svpg->rotationVariance = rotationVariance;
        svpg->isPaused = hardPaused;

        svpg->particleCount = particleCount;
        svpg->particleMeshId = particleMeshId;
        svpg->shaderID = shaderID;
        
        std::vector<VolumetricParticle*> startingParticleVector;

        std::vector<b_ParticleDynamic> dynamicDataVector;


        for (int i = 0; i<particleCount; i++) {
            VolumetricParticle* vp = new VolumetricParticle();
            vp->lifes = 0;
            vp->isPaused=hardPaused;
            this->restartSphericalGeneratorParticle(svpg, vp);
            startingParticleVector.push_back(vp);
            dynamicDataVector.push_back({
                vp->transform.getPosition(), vp->transform.getScale()
            });
        }
        
        svpg->particlePool = startingParticleVector;

        GLuint dynamicSSBO;
        glGenBuffers(1, &dynamicSSBO);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dynamicSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(b_ParticleDynamic) * particleCount, dynamicDataVector.data(), GL_DYNAMIC_DRAW);

        svpg->dynamicSSBO = dynamicSSBO;

        this->sphericalSourceMap[id] = svpg;
        
        return {svpg, id};
    };


    std::vector<SphericalVolumetricParticleGenerator*> VolumetricParticleGeneratorRepository::getAllSphericalGenerators() {
        std::vector<SphericalVolumetricParticleGenerator*> generators;
        for (auto p : this->sphericalSourceMap) {
            generators.push_back(p.second); 
        }
        return generators;
    };

    bool VolumetricParticleGeneratorRepository::stepSphericalGenerator(EngineID generatorID, float deltaTime) {
        SphericalVolumetricParticleGenerator* generator = this->sphericalSourceMap[generatorID];
        std::vector<b_ParticleDynamic> dynamicParticleDataBufferData;
        bool skippedPausedParticle=false;
        for (VolumetricParticle* particle : generator->particlePool) {
            // this makes kind of a mess in the buffers but that doesn't matter cuz we don't even update the static buffers anyway : D
            if (!generator->isPaused && particle->isPaused) particle->isPaused = false;
            skippedPausedParticle |= particle->isPaused;
            if (particle->isPaused) continue;
            if (particle->lifes >= generator->maxParticleLifes && generator->maxParticleLifes != -1) continue;
            particle->velocity += particle->actingForce * deltaTime;
            particle->transform.setPosition(
               particle->transform.getPosition() + 
               particle->velocity * deltaTime 
            );
            if (
                glm::length2(particle->transform.getPosition() - particle->startingPosition) >= 
                generator->radius * generator->radius 
            ) {
                particle->lifes++;
                this->restartSphericalGeneratorParticle(generator, particle);
                if (generator->isPaused) {
                    particle->isPaused = true;
                }
            }
            if (!particle->isPaused)
                dynamicParticleDataBufferData.push_back({particle->transform.getPosition(), particle->transform.getScale()});
        }

        generator->killedParticles = generator->particlePool.size() - dynamicParticleDataBufferData.size();
        if (!skippedPausedParticle && dynamicParticleDataBufferData.size() == 0) return false;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, generator->dynamicSSBO);
        glBufferSubData(
            GL_SHADER_STORAGE_BUFFER, 
            0, 
            sizeof(b_ParticleDynamic) * generator->particleCount, 
            dynamicParticleDataBufferData.data()
        );
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, generator->dynamicSSBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, generator->staticSSBO);
        return true;
    };

    void VolumetricParticleGeneratorRepository::restartSphericalGeneratorParticle(
        SphericalVolumetricParticleGenerator* parentGenerator,
        VolumetricParticle* particle
    ) {
        // 0. boiler plate
        particle->startingPosition = parentGenerator->position;
        particle->actingForce = parentGenerator->actingForce;

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

    void VolumetricParticleGeneratorRepository::deleteSphericalGenerator(EngineID generatorID) {
        SphericalVolumetricParticleGenerator* generator = this->sphericalSourceMap[generatorID];
        for (auto particle : generator->particlePool) {
            delete particle;
        }
        this->sphericalSourceMap.erase(generatorID);
        delete generator;
    }

    void VolumetricParticleGeneratorRepository::resetGenerator(EngineID generatorID) {
        SphericalVolumetricParticleGenerator* generator = this->sphericalSourceMap[generatorID];
        for (auto particle : generator->particlePool) {
            this->restartSphericalGeneratorParticle(generator, particle);
        }
        generator->killedParticles = 0;
    };

    VolumetricParticleGeneratorRepository volumetricParticleGeneratorRepository = VolumetricParticleGeneratorRepository();
}