#pragma once
#include <Engine/Math/Transform.hpp>
#include <string>

namespace Engine
{
    class GameObject {
    public:
        Math::Transform transform;
        std::string label;
        GameObject();
        GameObject(std::string label);
    };
};
