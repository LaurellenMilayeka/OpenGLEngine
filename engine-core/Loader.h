#pragma once

#include <string>
#include <map>
#include "Model.h"

namespace Engine
{
	namespace InternalIO
	{

		class Loader
		{
		private:

			static std::string _mBasePath;

		public:

			static Engine::Graphics::Model LoadModel(std::string const& modelPath);
		};
	}
}