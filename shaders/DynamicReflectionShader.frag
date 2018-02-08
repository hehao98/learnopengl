#version 330 core

out vec4 fragColor;

in vec4 fragPosition;
in vec3 normal;
in vec2 texCoord;

uniform samplerCube reflectionBox;
uniform vec3 viewPos;
uniform vec3 reflectionBoxCenter;

void main() {
    vec3 viewDir = vec3(fragPosition) - viewPos;
    vec3 reflectDir = reflect(normalize(viewDir), normalize(normal));
	fragColor = texture(reflectionBox, reflectDir);
}
