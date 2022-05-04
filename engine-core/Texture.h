#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>

namespace Engine
{
	namespace Graphics
	{
		struct Texture
		{
		private:

			void LoadTexture(std::string const& texPath);

		public:

			GLuint ID;

			Texture(std::string const& texPath);
		};
	}
}