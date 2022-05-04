#include "ModelRenderer.h"

using namespace Engine::Components;

ModelRenderer::ModelRenderer(Engine::Entity::Entity* parent) : IComponent("ModelRenderer", parent)
{

}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::setModel(Engine::Graphics::Model const& model)
{
	_mModel = model;
}

Engine::Graphics::Model const& ModelRenderer::GetModel() const
{
	return _mModel;
}
