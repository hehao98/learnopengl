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
in vec4 fragPosLightSpace;

out vec4 fragColor;

uniform vec3 viewPos;
uniform int lightCount;
uniform Light light[10];
uniform Material material;
uniform sampler2D shadowMap;

float calcShadow(vec4 lightSpacePos, Light l)
{
    // perform perspective divide
    vec3 projCoords = lightSpacePos.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - 0.005 > pcfDepth ? 0.0 : 1.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
            shadow = 0.0;
    return shadow;
}

vec3 calcPointLight(Light l)
{
    // Calculate shadows
    float shadow = calcShadow(fragPosLightSpace, l);

    float distance = length(vec3(fragPosition) - l.position);
    float attenuation = 1.0f/(1.0f + l.constant + l.linear*distance + l.quadratic*distance*distance);

    vec3 ambientLight = attenuation * l.ambient * vec3(texture(material.diffuse, texCoord));

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(l.position - fragPosition.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = shadow * attenuation * diffuse * vec3(texture(material.diffuse, texCoord)) * l.diffuse;

    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - fragPosition.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specularLight = shadow * attenuation * specular * vec3(texture(material.specular,texCoord)) * l.specular;
    return ambientLight + diffuseLight + specularLight;
}

void main()
{
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    for (int i = 0; i < lightCount; ++i) {
        fragColor += vec4(calcPointLight(light[i]), 1.0);
    }
}


