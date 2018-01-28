#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main()
{
    vec3 ambientLight = light.ambient * material.ambient;

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = (diffuse * material.diffuse) * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = specular * material.specular * light.specular;

	fragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0);
 }
