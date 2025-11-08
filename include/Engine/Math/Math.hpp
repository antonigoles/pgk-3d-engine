#pragma once
#include <OpenGL.hpp>
#include <Engine/Misc/Types.hpp>

namespace Engine::Math
{
   glm::quat lookAtQuat(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
   void meshRotate(Mesh& mesh, glm::quat rot);
   glm::quat rotateTowards(glm::quat currentRot, glm::vec3 targetDir, float t);
   float getRandom(float min, float max);
}
