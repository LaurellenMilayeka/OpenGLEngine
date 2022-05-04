#pragma once

#include <string>
#include <map>
#include "Model.h"

namespace Engine
{
	namespace InternalIO
	{

		enum class ModelType
		{
			FBX,
			OBJ,
			PMX,
			DAE
		};

		template<ModelType>
		class Loader
		{
		private:

			static std::string _mBasePath;

		public:

			static Engine::Graphics::Model LoadModel(std::string const& modelPath);
		};
	}
}