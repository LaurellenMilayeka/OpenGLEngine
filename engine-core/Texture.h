#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>

namespace Engine
{
	namespace Managers
	{
		class TextureManager;
	};

	namespace Graphics
	{
		enum class TextureType {
			TEXTURE_2D,
			TEXTURE_3D
		};

		struct Texture
		{
			friend class Engine::Managers::TextureManager;

		private:

			Texture(unsigned int id, TextureType type);

		public:

			GLuint ID;
			TextureType Type;
		};
	}
}