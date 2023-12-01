#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

			static std::map<std::string, unsigned int> _mBoneMapping;

			static void ExtractMaterials(std::vector<Engine::Graphics::Material*>& materialList, std::string const& basePath, const aiScene* scene);
			static void ExtractNodes(Engine::Graphics::Model& model, std::vector<Engine::Graphics::Material*>& materialList, aiNode* node, const aiScene* scene);
			static void ExtractMeshes(Engine::Graphics::Mesh* mesh, std::vector<Engine::Graphics::Material*>& materialList, aiMesh const* assimpMesh, aiScene const* scene);

		public:

			static Engine::Graphics::Model LoadModel(std::string const& modelPath);
		};
	}
}