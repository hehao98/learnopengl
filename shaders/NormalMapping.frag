#version 330 core

struct Light {
    vec4 pos;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

in VS_OUT {
    vec4 fragPos;
    vec2 texCoord;
    vec3 normal;
} fs_in;

out vec4 fragColor;

uniform vec4 viewPos;
uniform Light light;
uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform mat4 model;

// Implement BlinnPhong Shading
vec3 calcLight(Light l)
{
    vec3 n = normalize(texture(normalMap, fs_in.texCoord).rgb * 2.0 + 1.0);
    vec3 lightDir = normalize(vec3(l.pos - fs_in.fragPos));
    vec3 viewDir = normalize(vec3(viewPos - fs_in.fragPos));
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 ambient = l.ambient * vec3(texture(diffuse, fs_in.texCoord));
    vec3 diffuse = l.diffuse * max(dot(lightDir, n), 0.0) * vec3(texture(diffuse, fs_in.texCoord));
    vec3 specular = pow(max(dot(n, halfwayDir), 0.0), 64.0f) * l.color;
    return ambient + diffuse + specular;
}

void main()
{
	fragColor = vec4(calcLight(light), 1.0);
}
