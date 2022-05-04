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
