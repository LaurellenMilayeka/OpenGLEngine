#pragma once

#include <string>

namespace Engine
{
	namespace Graphics
	{
		enum ImageFormat
		{
			RGB8,
			RGBA8,
			ARGB8
		};

		struct Image
		{
			std::string Name;
			bool Exists;
		};
	}
}