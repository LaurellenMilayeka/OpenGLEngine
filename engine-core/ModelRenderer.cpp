#include "ModelRenderer.h"

#include <imgui.h>

using namespace Engine::Components;

ModelRenderer::ModelRenderer(Engine::Entity::Entity* parent) : IComponent("ModelRenderer", ComponentsID::ModelRenderer, parent)
{

}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::setModel(Engine::Graphics::Model const& model)
{
	_mModel = model;
}

#ifdef _DEBUG

Engine::Graphics::Model& ModelRenderer::GetModel()
{
	return _mModel;
}

#elif

Engine::Graphics::Model const& ModelRenderer::GetModel() const
{
	return _mModel;
}

#endif
