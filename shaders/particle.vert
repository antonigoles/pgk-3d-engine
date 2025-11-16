#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

struct DynamicData {
    vec3 pos;
    float scale;
};

layout(std430, binding = 1) readonly buffer DynamicBuffer {
    DynamicData dataDynamic[];
};

void main()
{
    vec3 world = aPos;
    world = world * dataDynamic[gl_InstanceID].scale;
    world += dataDynamic[gl_InstanceID].pos;

    gl_Position = projection * view * vec4(world, 1.0);
}