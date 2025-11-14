#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

struct StaticData {
    vec4 rot;
    float scale;
    float padding[3];
};

struct DynamicData {
    vec3 pos;
    float padding;
};

layout(std430, binding = 0) readonly buffer StaticBuffer {
    StaticData dataStatic[];
};

layout(std430, binding = 1) readonly buffer DynamicBuffer {
    DynamicData dataDynamic[];
};

vec3 quatRotate(vec4 q, vec3 v) {
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main()
{
    vec3 world = aPos;
    world = world * dataStatic[gl_InstanceID].scale;
    world = quatRotate(dataStatic[gl_InstanceID].rot, world);
    world += dataDynamic[gl_InstanceID].pos;

    gl_Position = projection * view * vec4(world, 1.0);
}