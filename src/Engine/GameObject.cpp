#include <Engine/Math/Transform.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Math/Transform.hpp>
#include <string>

namespace Engine
{
    GameObject::GameObject() {
        transform = Math::Transform();
        this->label = "GameObject";
    };

    GameObject::GameObject(std::string label) {
        this->label = label;
    }
};
