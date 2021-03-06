#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // Paramertes to determine its intensity over range.
    float constant;
    float linear;
    float quadratic;
};

in vec3 normal;
in vec2 texCoord;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main()
{
    float distance = length(vec3(fragPosition) - light.position);
    float attenuation = 1.0f/(1.0f + light.constant
            + light.linear*distance + light.quadratic*distance*distance);

    vec3 ambientLight = attenuation * light.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = attenuation * diffuse * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = attenuation * specular * vec3(texture(material.specular,texCoord)) * light.specular;

    vec3 emissionLight = vec3(texture(material.emission, texCoord));
	fragColor = vec4(ambientLight + diffuseLight + specularLight + emissionLight, 1.0);
 }
