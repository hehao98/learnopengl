#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

in vec3 normal;
in vec2 texCoord;
in vec4 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Material material;

void main()
{
    fragColor = texture(material.diffuse, texCoord);
    if (fragColor.a < 0.1) discard;
 }
