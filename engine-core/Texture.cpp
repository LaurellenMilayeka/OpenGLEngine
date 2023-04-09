#include "Texture.h"

extern "C" {
#include "stb_init.h"
}

using namespace Engine::Graphics;

Texture::Texture(std::string const& texPath, TextureType type)
{
	TextureName = texPath;
	Type = type;

	glGenTextures(1, &ID);
	LoadTexture(texPath, type);
}

void Texture::LoadTexture(std::string const& texPath, TextureType type)
{
	int width, height, nrChannels;
	unsigned char* data;

	data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
	if (data != nullptr)
	{
		GLint texFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		GLenum target = (type == TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D;
		glBindTexture(target, ID);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(target, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
		glBindTexture(target, 0);
	}
	else
	{
		std::cout << "Could not load texture" << std::endl;
	}
	stbi_image_free(data);
}