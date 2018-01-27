#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightSource;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Do the lighting calculation here
	vec3 normal = mat3(transpose(inverse(model))) * aNormal;
	vec4 position = model * vec4(aPos, 1.0);

	float ambient = 0.2f;
    vec3 ambientLight = ambient * lightColor;

    vec3 n = normalize(normal);
    vec3 lightDir = normalize(lightSource - position.xyz);
    float diffuse = max(dot(lightDir, n), 0.0f);
    vec3 diffuseLight = diffuse * lightColor;

    float specularStrength = 0.8f;
    vec3 reflectDir = normalize(reflect(-lightDir, n));
    vec3 viewDir = normalize(viewPos - position.xyz);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0f), 8);
    vec3 specularLight = specularStrength * specular * lightColor;
    color = vec4((ambientLight + diffuseLight + specularLight) * objectColor, 1.0);
}
