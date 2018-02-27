#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec4 fragPosition;
out vec2 texCoord;

uniform mat4 model[5];
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);
	normal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal;
	fragPosition = model[gl_InstanceID] * vec4(aPos, 1.0);
    texCoord = aTexCoord;
}
