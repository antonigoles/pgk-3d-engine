#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <Engine/Camera.hpp>
#include <Engine/TpsCamera.hpp>
#include <OpenGL.hpp>
#include <Engine/Math/Generator.hpp>
#include <Game/Scripts/Scripts.hpp>
#include <Engine/Math/Math.hpp>
#include <Engine/FpsCamera.hpp>
#include <Engine/VolumetricParticles.hpp>
#include <Engine/GameObjectCluster.hpp>
#include <Engine/Colisions.hpp>
#include <Engine/AsteroidExplosionService.hpp>
#include <Engine/GameEndService.hpp>

namespace Game
{
    Engine::Window* init(int v_width, int v_height, int N, int seed) {
        // set random Seed
        Engine::Math::seed = seed;

        // we do all the initializer stuff here
        Engine::Window* window = new Engine::Window(v_width, v_height);        

        // 1. Create a mesh
        Engine::EngineID diamondMesh = Engine::meshRepository.loadMeshFromFile("meshes/diamond.3d");
        Engine::EngineID cubeMesh = Engine::meshRepository.loadMeshFromFile("meshes/cube.3d");
        Engine::EngineID rocketMesh = Engine::meshRepository.loadMeshFromFileWithPreprocessing(
            "meshes/rocket.3d", 
            glm::angleAxis( glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
        );
        Engine::EngineID betterCubeMesh = Engine::meshRepository.loadMeshWithNormals({
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        });
        Engine::EngineID planeMesh = Engine::meshRepository.loadMeshFromFile("meshes/plane.3d");

        Engine::EngineID sphereMesh = Engine::meshRepository.loadMesh(Engine::Math::generateSphere(8));
        Engine::EngineID inverseSphereMesh = Engine::meshRepository.loadMesh(Engine::Math::generateInverseSphere(16));
    
        // // 2. Load some shaders
        Engine::EngineID asteroidShader = Engine::shaderRepository.loadShaderProgramAs(
            "asteroidShader",
            "shaders/asteroid.vert",
            "shaders/asteroid.frag"
        );

        Engine::EngineID lightShader = Engine::shaderRepository.loadShaderProgramAs(
            "lightShader",
            "shaders/light.vert",
            "shaders/light.frag"
        );

        Engine::EngineID floorShader = Engine::shaderRepository.loadShaderProgramAs(
            "floorShader",
            "shaders/floor.vert",
            "shaders/floor.frag"
        );

        Engine::EngineID skyboxShader = Engine::shaderRepository.loadShaderProgramAs(
            "skyboxShader",
            "shaders/sky.vert",
            "shaders/sky.frag"
        );

        Engine::EngineID playerShader = Engine::shaderRepository.loadShaderProgramAs(
            "skyboxShader",
            "shaders/player.vert",
            "shaders/player.frag"
        );

        std::vector<Engine::EngineID> asteroidMeshPool;
        std::vector<Engine::GameObjectCluster*> clusterPool;

        for (int i = 0; i<5; i++) {
            asteroidMeshPool.push_back(Engine::meshRepository.loadMesh(Engine::Math::generateMessySphere(8, i * seed)));
            auto cluster = Engine::gameObjectClusterRepository.createCluster();
            cluster.first->setMesh(asteroidMeshPool.back());
            cluster.first->setShader(asteroidShader);
            clusterPool.push_back(cluster.first);
        }

        // // 3. Create game object
        std::vector<glm::vec3> positionsPool;
        const float a1 = 3.0f;
        const float r = 2.0f * (N*N*N - a1);
        
        for (int j = 0; j<N; j++) {
            int ai = a1 + (float)j * r;
            int k = pow(ai, 0.333f) + 1;
            for (int i = 0; i<ai; i++) {
                // float xf = (((float)((i/(k*k)) % k) / (float)(k)));
                float yf = ((float)(i % k) / (float)(k)) * 2.0f * 3.14f;
                float zf = ((float)((i/k) % k) / (float)(k)) * 2.0f * 3.14f;

                glm::vec3 randomPosition = glm::normalize(glm::quat(
                    glm::vec3{
                        0.0f,
                        zf,
                        yf
                    }
                )) * glm::vec3{1.0f, 0.0f, 0.0f} * (10.0f * j + 20.0f);

                positionsPool.push_back(randomPosition);
            }
        }

        std::vector<glm::vec3> choices;
        int k = N;
        for (int i = 0; i<N*N*N; i++) {

            int choice = Engine::Math::getRandom(0.0, positionsPool.size()-1);
            glm::vec3 randomPosition = positionsPool[choice];
            
            do {
                std::swap(positionsPool[choice], positionsPool[positionsPool.size()-1]);
                positionsPool.pop_back();
                bool found = false;
                for (auto& v : choices) {
                    if (glm::length(v - randomPosition) <= 10.0f) {
                        choice = Engine::Math::getRandom(0.0, positionsPool.size()-1);
                        randomPosition = positionsPool[choice];
                        found = true;
                    }
                }
                if (!found) break;
            } while(true);

            choices.push_back(randomPosition);

            Engine::EngineID rockMesh = asteroidMeshPool[i % asteroidMeshPool.size()];
            auto e = Engine::gameObjectRepository.createEmptyGameObject();
            clusterPool[i % asteroidMeshPool.size()]->addObject(e.first);

            e.first->transform.setPosition(randomPosition);
            e.first->transform.setScale(3.0f * Engine::Math::getRandom(1.0, 1.7f));

            e.first->registerUpdateFunction(updateBall);
            e.first->set_float("index", (float)i);
            Engine::singleOBBDynamicMultiSphericalColiderColisionService.addStatic(e.first, e.first->transform.getScale());
        }

        for (auto cluster : clusterPool) cluster->initiateSSBO();

        auto lightSource = Engine::gameObjectRepository.createGameObject(sphereMesh, lightShader);
        lightSource.first->transform.setPosition({0.0f, 0.0f, -150.0f});
        lightSource.first->transform.setScale(12.5);

        // auto plane = Engine::gameObjectRepository.createGameObject(planeMesh, floorShader);
        // plane.first->transform.setPosition({0.0f, 0.0f, 0.0f});
        // plane.first->transform.setScale(250.0);

        auto skybox = Engine::gameObjectRepository.createGameObject(inverseSphereMesh, skyboxShader);
        skybox.first->transform.setScale(250.0f);

        // // 4. Create camera and move it back a little bit
        Engine::Camera* tpsCamera = new Engine::TpsCamera(120.0f);
        Engine::Camera* fpsCamera = new Engine::FpsCamera(90.0f);
        Engine::Player* player = new Engine::Player();
        player->transform.setPosition({0.0f, 0.0f, 0.0f});

        auto playerGameObject = Engine::gameObjectRepository.createGameObject(rocketMesh, playerShader);

        // calculate player mesh cuboid
        auto cuboid = Engine::meshRepository.getMeshCuboid(rocketMesh, 0.0);
        Engine::singleOBBDynamicMultiSphericalColiderColisionService.setDynamic(playerGameObject.first, cuboid.first, cuboid.second);
        
        player->setPlayerGameObject(playerGameObject.first);
        playerGameObject.first->registerUpdateFunction(updatePlayerGameObject);
        // For FPS camera hide, for tps show
        // playerGameObject.first->hide();

        window->setCamera1(tpsCamera);
        window->setCamera2(fpsCamera);
        window->setPlayer(player);
        window->setSkybox(skybox.first);


        Engine::EngineID particleShader = Engine::shaderRepository.loadShaderProgramAs(
            "particleShader",
            "shaders/particle.vert",
            "shaders/particle.frag"
        );

        Engine::EngineID asteroidParticleShader = Engine::shaderRepository.loadShaderProgramAs(
            "asteroidParticle",
            "shaders/asteroidParticle.vert",
            "shaders/asteroidParticle.frag"
        );

        Engine::EngineID particleMesh = Engine::meshRepository.loadMesh({
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        });

        auto rocketParticlesGenerator = Engine::volumetricParticleGeneratorRepository.createSphericalParticleSource(
            glm::vec3{0,0,2.0f}, // position
            glm::vec3{0,0,10.0f}, // acting force
            -1,
            16.0f, // radius
            4.0f, // particleSpeed
            0.1f, // speedVariance
            0.4f, // particleSize
            0.6f, // sizeVariance
            1.0f, // rotationVariance
            100, // particle count
            sphereMesh,
            particleShader,
            true
        );

        playerGameObject.first->set_ref("particleGenerator", rocketParticlesGenerator.first);
        playerGameObject.first->set_ref("playerObject", player);

        Engine::asteroidExplosionService.setExplosionParticleMesh(asteroidMeshPool[0]);
        Engine::asteroidExplosionService.setExplosionParticleShader(asteroidParticleShader);

        Engine::gameEndService.updateGameEndActors({
            playerGameObject.first,
            lightSource.first
        });

        return window;
    };
};
