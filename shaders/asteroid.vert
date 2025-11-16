#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vecpos;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;

struct SSBO_Transform {
    // vec3 position; 
    // vec4 rotation;
    // float scale; 
    mat4 model;
};

layout(std430, binding = 0) readonly buffer TransformBuffer {
    SSBO_Transform transformData[];
};

// vec3 quatRotate(vec4 q, vec3 v) {
//     return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
// };

void main()
{
    // vec3 world = aPos;
    // world = world * transformData[gl_InstanceID].scale;
    // world = quatRotate(transformData[gl_InstanceID].rotation, world);
    // world += transformData[gl_InstanceID].position;

    // gl_Position = projection * view * vec4(world, 1.0);
    // FragPos = world; 
    // vecpos = aPos;
    // Normal = mat3(transpose(inverse(model))) * aNormal;  

    mat4 model = transformData[gl_InstanceID].model;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    vecpos = aPos;
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}