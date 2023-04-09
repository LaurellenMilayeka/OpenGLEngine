#include "Model.h"

using namespace Engine::Graphics;

void Model::UpdateModelData()
{
	for (Mesh& mesh : _mMeshGroup)
	{
		mesh.Compute();
	}
}

Model::Model(Engine::Misc::Shader const& shader)
{
	_mShader = shader;
}

Model::~Model()
{

}

std::string const& Model::GetName() const
{
	return _mName;
}

void Model::SetName(std::string const& newName)
{
	_mName = newName;
}

Mesh* Model::CreateMesh()
{
	_mMeshGroup.resize(_mMeshGroup.size() + 1);
	return &_mMeshGroup.back();
}

std::vector<Mesh> const& Model::GetMeshes() const
{
	return _mMeshGroup;
}

Engine::Misc::Shader const& Model::GetShader() const
{
	return _mShader;
}

void Model::SetShader(Engine::Misc::Shader const& shader)
{
	_mShader = shader;
}
