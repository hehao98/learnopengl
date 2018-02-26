#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 normal[];
in vec2 texCoord[];
in vec4 fragPosition[];

out vec3 fNormal;
out vec4 fFragPosition;
out vec2 fTexCoord;

uniform float time;

vec3 getNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 n)
{
    float magnitude = 2.0;
    vec3 offset = normalize(n) * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(offset, 1.0);
}

void main() {
    vec4 pos;
    vec3 n = getNormal();
    pos = gl_in[0].gl_Position;
    gl_Position = explode(pos, n);
    fNormal = normal[0];
    fFragPosition = fragPosition[0];
    fTexCoord = texCoord[0];
    EmitVertex();
    pos = gl_in[1].gl_Position;
    gl_Position = explode(pos, n);
    fNormal = normal[1];
    fFragPosition = fragPosition[1];
    fTexCoord = texCoord[1];
    EmitVertex();
    pos = gl_in[2].gl_Position;
    gl_Position = explode(pos, n);
    fNormal = normal[2];
    fFragPosition = fragPosition[2];
    fTexCoord = texCoord[2];
    EmitVertex();
}