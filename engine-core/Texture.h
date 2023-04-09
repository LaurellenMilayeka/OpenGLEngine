#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>

namespace Engine
{
	namespace Graphics
	{
		enum class TextureType {
			TEXTURE_2D,
			TEXTURE_3D
		};

		struct Texture
		{
		private:

			std::string TextureName;

			void LoadTexture(std::string const& texPath, TextureType type);

		public:

			GLuint ID;
			TextureType Type;

			Texture(std::string const& texPath, TextureType type);
		};
	}
}