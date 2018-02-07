#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
	fragColor = texture(screenTexture, TexCoords);
}
