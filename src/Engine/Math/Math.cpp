#include <Engine/Math/Math.hpp>
#include <OpenGL.hpp>
#include <Engine/Misc/Types.hpp>
#include <random>

namespace Engine::Math 
{
    void meshRotate(Mesh& mesh, glm::quat rot) {
        for (int i = 0; i<mesh.size(); i+=3) {
            glm::vec3 v = {mesh[i], mesh[i+1], mesh[i+2]};
            v = rot * v;
            mesh[i] = v.x;
            mesh[i+1] = v.y;
            mesh[i+2] = v.z;
        }
    };

    glm::quat lookAtQuat(glm::vec3 eye, glm::vec3 target, glm::vec3 up) {
        glm::vec3 f = glm::normalize(target - eye);
        glm::vec3 r = glm::normalize(glm::cross(f, up));
        glm::vec3 u = glm::cross(r, f);
        glm::mat3 rot(r, u, -f);
        return glm::quat_cast(rot);
    };

    glm::quat rotateTowards(glm::quat currentRot, glm::vec3 targetDir, float t) {
        glm::vec3 fromDir = glm::normalize(currentRot * glm::vec3(0, 0, -1));
        glm::vec3 toDir   = glm::normalize(targetDir);

        float dot = glm::dot(fromDir, toDir);
        glm::quat targetRot;

        if (dot < -0.9999f) {
            glm::vec3 ortho = glm::normalize(glm::cross(glm::vec3(1, 0, 0), fromDir));
            if (glm::length(ortho) < 1e-6f)
                ortho = glm::normalize(glm::cross(glm::vec3(0, 1, 0), fromDir));
            targetRot = glm::angleAxis(glm::radians(180.0f), ortho);
        } else {
            glm::vec3 axis = glm::normalize(glm::cross(fromDir, toDir));
            float angle = acosf(glm::clamp(dot, -1.0f, 1.0f));
            targetRot = glm::angleAxis(angle, axis);
        }

        targetRot = targetRot * currentRot;      // absolutna rotacja
        glm::quat smoothRot = glm::slerp(currentRot, targetRot, glm::clamp(t, 0.0f, 1.0f));

        return glm::normalize(smoothRot);
    }

    float getRandom(float min, float max) {
        static std::random_device rd;  // Źródło entropii (używane tylko raz)
        static std::mt19937 gen(rd()); // Generator Mersenne Twister
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};
