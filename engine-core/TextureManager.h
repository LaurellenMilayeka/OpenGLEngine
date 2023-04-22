#pragma once

#include <string>
#include <map>

#include "Texture.h"

namespace Engine
{
	namespace Graphics
	{
		struct Texture;
	};

	namespace Managers
	{
		class TextureManager
		{
		private:

			static std::map<std::string, Engine::Graphics::Texture*> _loadedTextures;

		public:

			static Engine::Graphics::Texture* CreateTexture(std::string const& texPath, Engine::Graphics::TextureType const type);

			static void FreeTextures();
		};
	};
};