#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace Engine
{
    typedef std::vector<float> Mesh;
    typedef unsigned int EngineID;

    // GameObject *gameObject, float deltaTime
    class UpdateFunctionData {
    public:
        void* sourcePointer;
        float deltaTime;
    };

    typedef std::function<void(UpdateFunctionData)> GameObjectUpdateFunction;
};

