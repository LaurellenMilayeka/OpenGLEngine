#pragma once

#include <cstdint>
#include <vector>
#include <GL/glew.h>

#include "Shader.h"
#include "Mesh.h"

namespace Engine
{
	namespace Graphics
	{
		class Model
		{
		private:

			std::string       _mName;
			std::vector<Mesh> _mMeshGroup;

			Engine::Misc::Shader _mShader;


		public:

			Model() = default;
			Model(Engine::Misc::Shader const& shader);
			~Model();

			Mesh* CreateMesh();

			std::string const& GetName() const;
			void SetName(std::string const& newName);

			std::vector<Mesh> const& GetMeshes() const;

			Engine::Misc::Shader const& GetShader() const;
			void SetShader(Engine::Misc::Shader const& shader);

			void UpdateModelData();
		};
	}
}

