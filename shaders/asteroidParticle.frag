#version 460 core

in vec3 vecpos;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(0.5, 0.3, 0.1), 0.8f);
}
