#pragma once
#include <Engine/Math/Transform.hpp>
#include <Engine/Misc/Types.hpp>
#include <string>

namespace Engine
{
    class GameObject {
    private:
        bool v_isVisible;
        std::vector<GameObjectUpdateFunction> updateFunctions;

        std::unordered_map<std::string, glm::vec3> vec3map;
        std::unordered_map<std::string, float> floatmap;
    public:
        Math::Transform transform;
        std::string label;
        GameObject();
        GameObject(std::string label);
        
        void registerUpdateFunction(GameObjectUpdateFunction updateFunction);
        void callUpdateFunctions(float deltaTime);
        
        void hide();
        void show();
        bool isVisible();

        void set_vec3(const std::string &label, glm::vec3 val);
        glm::vec3 get_vec3(const std::string &label);
        bool has_vec3(const std::string &label);

        void set_float(const std::string &label, float val);
        float get_float(const std::string &label);
        bool has_float(const std::string &label);
    };
};
