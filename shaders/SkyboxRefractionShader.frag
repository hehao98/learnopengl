#version 330 core

out vec4 fragColor;

in vec4 fragPosition;
in vec3 normal;
in vec2 texCoord;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main() {
    vec3 viewDir = vec3(fragPosition) - viewPos;
    vec3 refractDir = refract(normalize(viewDir), normalize(normal), 1.0/1.53);
	fragColor = texture(skybox, refractDir);
}
