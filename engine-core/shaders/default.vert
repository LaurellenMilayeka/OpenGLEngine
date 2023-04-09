#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

out vec2 AmbientTexCoord;
out vec2 DiffuseTexCoord;
out vec2 SpecularTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = vec3(aTexCoord.x, 1.0 - aTexCoord.y, aTexCoord.z);
    Normal = aNormal;
}