#version 460 core

in vec3 vecpos;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
