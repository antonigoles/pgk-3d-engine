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

        std::vector<b_ParticleStatic> staticDataVector;
        std::vector<b_ParticleDynamic> dynamicDataVector;


        for (int i = 0; i<particleCount; i++) {
            VolumetricParticle* vp = new VolumetricParticle();
            this->restartSphericalGeneratorParticle(svpg, vp);
            startingParticleVector.push_back(vp);

            auto rot = vp->transform.getRotation();
            staticDataVector.push_back({
                glm::vec4(rot.x, rot.y, rot.z, rot.w),
                vp->transform.getScale(),
                0,0,0
            });

            dynamicDataVector.push_back({
                vp->transform.getPosition(), 0
            });
        }
        
        svpg->particlePool = startingParticleVector;

        GLuint dynamicSSBO, staticSSBO;
        glGenBuffers(1, &dynamicSSBO);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dynamicSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(b_ParticleDynamic) * particleCount, dynamicDataVector.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, dynamicSSBO);

        glGenBuffers(1, &staticSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, staticSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(b_ParticleStatic) * particleCount, staticDataVector.data(), GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, staticSSBO);


        svpg->staticSSBO = staticSSBO;
        svpg->dynamicSSBO = dynamicSSBO;

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
        std::vector<b_ParticleDynamic> dynamicParticleDataBufferData;
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
            dynamicParticleDataBufferData.push_back({particle->transform.getPosition(), 0});
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, generator->dynamicSSBO);
        glBufferSubData(
            GL_SHADER_STORAGE_BUFFER, 
            0, 
            sizeof(b_ParticleDynamic) * generator->particleCount, 
            dynamicParticleDataBufferData.data()
        );
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