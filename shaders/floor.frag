#version 330 core

in vec3 vecpos;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
}
