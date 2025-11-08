#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <Engine/Camera.hpp>
#include <Engine/TpsCamera.hpp>
#include <OpenGL.hpp>
#include <Engine/Math/Generator.hpp>
#include <Game/Scripts/Scripts.hpp>
#include <Engine/Math/Math.hpp>

#include <Engine/VolumetricParticles.hpp>


namespace Game
{
    Engine::Window* init(int v_width, int v_height) {
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

        Engine::EngineID sphereMesh = Engine::meshRepository.loadMesh(Engine::Math::generateSphere(64));
        Engine::EngineID inverseSphereMesh = Engine::meshRepository.loadMesh(Engine::Math::generateInverseSphere(64));
    
        // // 2. Load some shaders
        Engine::EngineID baseShader = Engine::shaderRepository.loadShaderProgramAs(
            "baseShader",
            "shaders/base.vert",
            "shaders/base.frag"
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
        for (int i = 0; i<10; i++) 
            asteroidMeshPool.push_back(Engine::meshRepository.loadMesh(Engine::Math::generateMessySphere(8, i * 1337)));

        // // 3. Create game object
        for (int i = 0; i<100; i++) {
            float rand1 = -30.0f + static_cast<float>(rand()) / RAND_MAX * (60.0f);
            float rand2 = -30.0f + static_cast<float>(rand()) / RAND_MAX * (60.0f);
            float randY = 2.0f + static_cast<float>(rand()) / RAND_MAX * (20.0f);
            Engine::EngineID rockMesh = asteroidMeshPool[i % asteroidMeshPool.size()];
            auto e = Engine::gameObjectRepository.createGameObject(rockMesh, baseShader);
            e.first->transform.setPosition({ rand1, randY, rand2 });
            e.first->transform.setScale(1.0f);
            e.first->registerUpdateFunction(updateBall);
            e.first->set_float("index", (float)i);
        }

        auto lightSource = Engine::gameObjectRepository.createGameObject(sphereMesh, lightShader);
        lightSource.first->transform.setPosition({0.0f, 0.0f, -150.0f});
        lightSource.first->transform.setScale(12.5);

        // auto plane = Engine::gameObjectRepository.createGameObject(planeMesh, floorShader);
        // plane.first->transform.setPosition({0.0f, 0.0f, 0.0f});
        // plane.first->transform.setScale(250.0);

        auto skybox = Engine::gameObjectRepository.createGameObject(inverseSphereMesh, skyboxShader);
        skybox.first->transform.setScale(250.0f);

        // // 4. Create camera and move it back a little bit
        Engine::Camera* camera = new Engine::TpsCamera(120.0f);
        Engine::Player* player = new Engine::Player();
        player->transform.setPosition({0.0f, 3.0f, 20.0f});

        auto playerGameObject = Engine::gameObjectRepository.createGameObject(rocketMesh, playerShader);
        player->setPlayerGameObject(playerGameObject.first);
        playerGameObject.first->registerUpdateFunction(updatePlayerGameObject);
        // For FPS camera hide, for tps show
        // playerGameObject.first->hide();

        window->setCamera(camera);
        window->setPlayer(player);
        window->setSkybox(skybox.first);


        Engine::EngineID particleShader = Engine::shaderRepository.loadShaderProgramAs(
            "particleShader",
            "shaders/particle.vert",
            "shaders/particle.frag"
        );

        Engine::EngineID particleMesh = Engine::meshRepository.loadMesh({
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        });

        Engine::volumetricParticleGeneratorRepository.createSphericalParticleSource(
            glm::vec3{0,0,0}, // position
            glm::vec3{0,-4.0f,0}, // acting force
            15.0f, // radius
            4.0f, // particleSpeed
            0.2f, // speedVariance
            0.2f, // particleSize
            0.1f, // sizeVariance
            1.0f, // rotationVariance
            1000, // particle count
            particleMesh,
            particleShader
        );

        return window;
    };
};
