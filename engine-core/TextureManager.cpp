#include "TextureManager.h"

extern "C" {
#include "stb_init.h"
}

using namespace Engine::Managers;

std::map<std::string, Engine::Graphics::Texture*> TextureManager::_loadedTextures;

Engine::Graphics::Texture* TextureManager::CreateTexture(std::string const& texPath, Engine::Graphics::TextureType const type)
{
	int width, height, nrChannels;
	unsigned char* data;
	unsigned ID = 0;
	std::map<std::string, Engine::Graphics::Texture*>::const_iterator it;

	if ((it = _loadedTextures.find(texPath)) != _loadedTextures.end())
	{
		return _loadedTextures[texPath];
	}
	else
	{
		data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
		if (data != nullptr)
		{
			GLint texFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
			GLenum target = (type == Engine::Graphics::TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D;
			glBindTexture(target, ID);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(target, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(target);
			glBindTexture(target, 0);

			_loadedTextures[texPath] = new Engine::Graphics::Texture(ID, type);
			return _loadedTextures[texPath];
		}
		else
		{
			std::cout << "Could not load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}

void TextureManager::FreeTextures()
{

}