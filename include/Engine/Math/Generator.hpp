#pragma once
#include <Engine/Misc/Types.hpp>

namespace Engine::Math {
    Mesh generateSphere(int precision);
    Mesh generateMessySphere(int precision, int seed);
    Mesh generateInverseSphere(int precision);
    float perlinNoise3D(const glm::vec3& pos, unsigned int seed);
};