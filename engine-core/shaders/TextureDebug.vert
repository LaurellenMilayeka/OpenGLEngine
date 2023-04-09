#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 PosCoord;
out vec3 NorCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTexCoord;
    PosCoord = aPos;
    NorCoord = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}