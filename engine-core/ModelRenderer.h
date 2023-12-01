#pragma once

#include <GL/glew.h>
#include <imgui.h>

#include "IComponent.h"
#include "Model.h"

namespace Engine
{
	namespace Components
	{
		class ModelRenderer : public IComponent
		{
		private:

			Engine::Graphics::Model _mModel;

		public:

			ModelRenderer(Engine::Entity::Entity* parent);
			virtual ~ModelRenderer();

			void setModel(Engine::Graphics::Model const& model);

#ifdef _DEBUG
			Engine::Graphics::Model& GetModel();
#elif
			Engine::Graphics::Model const& GetModel() const;
#endif
			virtual void SetupDebugWindow() override
			{
				if (ImGui::TreeNode(_mModel.GetName().c_str()))
				{
					for (Engine::Graphics::Mesh& mesh : _mModel.GetMeshes())
					{
						ImGui::Checkbox(mesh.GroupName.c_str(), &mesh.IsEnabled);
					}
					ImGui::TreePop();
				}
			}

		};
	}
}

