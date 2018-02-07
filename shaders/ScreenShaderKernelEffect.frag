#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
	float offset = 1.0/300.0;
	vec2 offsets[] = vec2[](
	    vec2(-offset, offset),
	    vec2(0, offset),
	    vec2(offset, offset),
	    vec2(-offset, 0),
	    vec2(0, 0),
	    vec2(offset, 0),
	    vec2(-offset, -offset),
	    vec2(0, -offset),
	    vec2(offset, -offset)
	);
	float kernel[9] = float[](
	    -1, -1, -1,
	    -1,  9, -1,
	    -1, -1, -1
	);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; ++i)
	    sampleTex[i] = vec3(texture(screenTexture, TexCoords + offsets[i]));
	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; ++i)
	    color += kernel[i] * sampleTex[i];
    fragColor = vec4(color, 1.0);
}
