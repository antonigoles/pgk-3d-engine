#include <Engine/Math/Transform.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Math/Transform.hpp>
#include <string>

namespace Engine
{
    GameObject::GameObject() : GameObject("GameObject") {};

    GameObject::GameObject(std::string label) {
        transform = Math::Transform();
        this->label = label;
        this->v_isVisible = true;
        this->parentCluster = nullptr;
    }

    void GameObject::registerUpdateFunction(GameObjectUpdateFunction updateFunction) {
        this->updateFunctions.push_back(updateFunction);
    };

    void GameObject::callUpdateFunctions(float deltaTime) {
        for (auto fun : this->updateFunctions) fun({this, deltaTime});
    };

    void GameObject::hide() {
        this->v_isVisible = false;
    };

    void GameObject::show() {
        this->v_isVisible = true;
    };

    bool GameObject::isVisible() {
        return this->v_isVisible;
    };

    void GameObject::set_vec3(const std::string &label, glm::vec3 val) {
        this->vec3map[label] = val;
    };

    glm::vec3 GameObject::get_vec3(const std::string &label) {
        // TODO: make this secure but not slow
        return this->vec3map[label];
    };

    bool GameObject::has_vec3(const std::string &label) {
        return this->vec3map.contains(label);
    };

    void GameObject::set_float(const std::string &label, float val) {
        this->floatmap[label] = val;
    };

    float GameObject::get_float(const std::string &label) {
        // TODO: make this secure but not slow
        return this->floatmap[label];
    };

    bool GameObject::has_float(const std::string &label) {
        return this->floatmap.contains(label);
    };

    void GameObject::set_ref(const std::string &label, void* val) {
        this->refmap[label] = val;
    };

    void* GameObject::get_ref(const std::string &label) {
        return this->refmap[label];
    };

    bool GameObject::has_ref(const std::string &label) {
        return this->refmap.contains(label);
    };


    bool GameObject::hasParentCluster() {
        return this->parentCluster != nullptr;
    };

    void GameObject::setParentCluster(void* cluster) {
        this->parentCluster = cluster;
    };
};
