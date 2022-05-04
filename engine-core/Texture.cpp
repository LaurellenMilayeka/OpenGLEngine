#include "Texture.h"

extern "C" {
#include "stb_init.h"
}

using namespace Engine::Graphics;

Texture::Texture(std::string const& texPath)
{
	glGenTextures(1, &ID);
	LoadTexture(texPath);
}

void Texture::LoadTexture(std::string const& texPath)
{
	int width, height, nrChannels;
	unsigned char* data;

	data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
	if (data != nullptr)
	{
		GLint texFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Could not load texture" << std::endl;
	}
	stbi_image_free(data);
}