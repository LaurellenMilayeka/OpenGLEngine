#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

in vec3 vNormal;
in vec3 FragPos;

uniform bool hasLight;
uniform bool hasTexture;
uniform bool has3DTexture;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform sampler2D overallTexture2D;
uniform sampler3D overallTexture3D;

void main()
{
    if (hasTexture)
	{
		if (has3DTexture) {
			FragColor = texture(overallTexture3D, TexCoord);
		}
		else {
			FragColor = texture(overallTexture2D, TexCoord.xy);
		}
	}
	else
	{
		FragColor = texture(overallTexture2D, TexCoord.xy);
	}
}