#include <Engine/Window.hpp>
#include <Engine/Misc/Monoliths.hpp>
#include <Engine/Camera.hpp>
#include <OpenGL.hpp>
#include <Engine/Math/Generator.hpp>

namespace Engine
{
    Engine::Window* init(int v_width, int v_height) {
        // we do all the initializer stuff here
        Engine::Window* window = new Window(v_width, v_height);        

        // 1. Create a mesh
        EngineID diamondMesh = meshRepository.loadMeshFromFile("meshes/diamond.3d");
        EngineID cubeMesh = meshRepository.loadMeshFromFile("meshes/cube.3d");
        EngineID rocketMesh = meshRepository.loadMeshFromFile("meshes/rocket.3d");
        EngineID betterCubeMesh = meshRepository.loadMeshWithNormals({
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

        EngineID sphereMesh = meshRepository.loadMesh(Engine::Math::generateSphere(64));
    
        // // 2. Load some shaders
        EngineID baseShader = shaderRepository.loadShaderProgramAs(
            "baseShader",
            "shaders/base.vert",
            "shaders/base.frag"
        );

        EngineID lightShader = shaderRepository.loadShaderProgramAs(
            "lightShader",
            "shaders/light.vert",
            "shaders/light.frag"
        );

        // // 3. Create game object
        for (int i = 0; i<10; i++) {
            float rand1 = -10.0f + static_cast<float>(rand()) / RAND_MAX * (20.0f);
            float rand2 = -10.0f + static_cast<float>(rand()) / RAND_MAX * (20.0f);
            auto e = gameObjectRepository.createGameObject(sphereMesh, baseShader);
            e.first->transform.setPosition({ rand1, 0.0f, rand2 });
            e.first->transform.setScale(1.0f);
        }

        auto lightSource = gameObjectRepository.createGameObject(cubeMesh, lightShader);
        lightSource.first->transform.setPosition({1.2f, 1.0f, 2.0f});
        lightSource.first->transform.setScale(0.5);
        lightSource.first->label = "lightSource";

        // // 4. Create camera and move it back a little bit
        Camera* camera = new Camera(80.0f);
        camera->transform.setPosition({0.0f, 2.0f, 20.0f});

        window->setCamera(camera);

        return window;
    };
};
