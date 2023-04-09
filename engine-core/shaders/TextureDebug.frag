#version 330

in vec2 TexCoord;
in vec3 PosCoord;
in vec3 NorCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(NorCoord, 1.0f);
    FragColor = texture(ourTexture, TexCoord);
}