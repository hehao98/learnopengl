#version 330 core

layout (location = 0) in vec3 aPos;

uniform float horizonalOffset;

void main()
{
    gl_Position = vec4(aPos.x + horizonalOffset, aPos.y, aPos.z, 1.0);
}
