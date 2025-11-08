#version 330 core

in vec3 vecpos;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    FragColor = vec4(vec3(0.6, 0.3, 0.1), 1.0f);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // vec3 result = (ambient + diffuse) * FragColor;

    // FragColor = vec4(result.xyz, 1.0f);

    float specularStrength = 0.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    
    float distanceFromLight = pow(2, length(viewPos - FragPos)/8);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), distanceFromLight);
    vec3 specular =  (specularStrength * spec * lightColor);  
    vec3 result = (ambient + diffuse + specular) * FragColor.xyz;
    FragColor = vec4(result, 1.0);
}
