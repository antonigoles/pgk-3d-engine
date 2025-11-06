#include <Engine/Misc/Types.hpp>
#include <vector>
#include <glm/ext/quaternion_trigonometric.hpp>

namespace Engine::Math {
    Mesh generateSphere(int precision) {
        Mesh mesh;
        const float PI = 3.14159265359f;

        for (int i = 0; i < precision; i++) {
            float phi1 = PI * i / precision;
            float phi2 = PI * (i + 1) / precision;

            for (int j = 0; j < precision; j++) {
                float theta1 = 2.0f * PI * j / precision;
                float theta2 = 2.0f * PI * (j + 1) / precision;

                glm::vec3 p1(
                    sin(phi1) * cos(theta1),
                    cos(phi1),
                    sin(phi1) * sin(theta1)
                );
                glm::vec3 p2(
                    sin(phi2) * cos(theta1),
                    cos(phi2),
                    sin(phi2) * sin(theta1)
                );
                glm::vec3 p3(
                    sin(phi2) * cos(theta2),
                    cos(phi2),
                    sin(phi2) * sin(theta2)
                );
                glm::vec3 p4(
                    sin(phi1) * cos(theta2),
                    cos(phi1),
                    sin(phi1) * sin(theta2)
                );

                // trójkąt 1 (CCW)
                mesh.insert(mesh.end(), {p1.x, p1.y, p1.z});
                mesh.insert(mesh.end(), {p2.x, p2.y, p2.z});
                mesh.insert(mesh.end(), {p3.x, p3.y, p3.z});

                // trójkąt 2 (CCW)
                mesh.insert(mesh.end(), {p1.x, p1.y, p1.z});
                mesh.insert(mesh.end(), {p3.x, p3.y, p3.z});
                mesh.insert(mesh.end(), {p4.x, p4.y, p4.z});
            }
        }

        return mesh;
    }
};