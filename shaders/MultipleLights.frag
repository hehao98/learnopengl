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

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float innerCone;
    float outerCone;
};

in vec3 normal;
in vec2 texCoord;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Light light;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;

vec3 calcPointLight(Light l)
{
    float distance = length(vec3(fragPosition) - l.position);
    float attenuation = 1.0f/(1.0f + l.constant
            + l.linear*distance + l.quadratic*distance*distance);

    vec3 ambientLight = attenuation * l.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(l.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = attenuation * diffuse * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = attenuation * specular * vec3(texture(material.specular,texCoord)) * light.specular;
    return ambientLight + diffuseLight + specularLight;
}

vec3 calcDirLight(DirLight l)
{
    vec3 ambientLight = l.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(-l.direction); // Use position vector as direction
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = diffuse * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = specular * vec3(texture(material.specular,texCoord)) * light.specular;
    return ambientLight + diffuseLight + specularLight;
}

vec3 calcSpotLight(SpotLight l)
{
    float distance = length(vec3(fragPosition) - l.position);
    float attenuation = 1.0f/(1.0f + l.constant
            + l.linear*distance + l.quadratic*distance*distance);

    vec3 ambientLight = l.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(l.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = diffuse * vec3(texture(material.diffuse, texCoord)) * l.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = specular * vec3(texture(material.specular,texCoord)) * l.specular;

    float angle = dot(-lightDir, normalize(l.direction));
    float spotStrength = clamp((l.outerCone - angle) /
    (l.outerCone - l.innerCone), 0.0, 1.0);
    return attenuation * (ambientLight + spotStrength * (diffuseLight + specularLight));
}

void main()
{
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    fragColor += vec4(calcPointLight(light), 1.0);
    fragColor += vec4(calcDirLight(dirLight), 1.0);
    fragColor += vec4(calcSpotLight(spotLight), 1.0);
    vec3 emissionLight = vec3(texture(material.emission, texCoord));
	fragColor += vec4(emissionLight, 1.0);
 }
