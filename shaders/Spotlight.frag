#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCone;
    float outerCone;
};

in vec3 normal;
in vec2 texCoord;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;
uniform SpotLight light;
uniform Material material;

void main()
{

    vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = diffuse * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = specular * vec3(texture(material.specular,texCoord)) * light.specular;

    float angle = dot(-lightDir, normalize(light.direction));
    float spotStrength = clamp((light.outerCone - angle) /
    (light.outerCone - light.innerCone), 0.0, 1.0);
    vec3 lightingResult = ambientLight + spotStrength * (diffuseLight + specularLight);

    vec3 emissionLight = vec3(texture(material.emission, texCoord));
	fragColor = vec4(lightingResult + emissionLight, 1.0);
 }
