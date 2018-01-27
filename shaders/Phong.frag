#version 330 core

in vec3 normal;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightSource;
uniform vec3 viewPos;

void main()
{
    float ambient = 0.2f;
    vec3 ambientLight = ambient * lightColor;

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(lightSource - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = diffuse * lightColor;

    float specularStrength = 0.8f;
    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
    vec3 specularLight = specularStrength * specular * lightColor;
	fragColor = vec4((ambientLight + diffuseLight + specularLight) * objectColor, 1.0);
}
