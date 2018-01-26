#version 330 core

out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float visibility;

void main()
{
	fragColor = mix(texture(texture1, texCoord),
	                texture(texture2, vec2(-texCoord.x + 1.0, texCoord.y) * 2),
	                visibility);
}
