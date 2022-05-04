#include "Renderer.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include <GL/glew.h>
#include <vector>

using namespace Engine::Systems;

void Renderer::Update()
{
	std::vector<Engine::Entity::Entity*> entityList = Engine::Entity::EntityManager::_mEntityPool;
	Engine::Components::Camera* cam = Entity::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Camera>();
	Engine::Components::Transform* camTransform = Entity::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Transform>();
	Engine::Components::Transform* light = (Entity::EntityManager::Get("Light") != nullptr) ?
		Entity::EntityManager::Get("Light")->GetComponent<Engine::Components::Transform>()
		: nullptr;

	assert(cam != nullptr);

	for (Engine::Entity::Entity* item : entityList)
	{
		if (item->HasComponent(Engine::Components::ComponentsID::Transform)
			&& item->HasComponent(Engine::Components::ComponentsID::ModelRenderer))
		{
			Components::Transform* transform = item->GetComponent<Components::Transform>();
			Components::ModelRenderer* modelRenderer = item->GetComponent<Components::ModelRenderer>();

			if (light == nullptr)
			{
				modelRenderer->GetModel().GetShader().SetValue("hasLight", false);
			}
			else
			{
				modelRenderer->GetModel().GetShader().SetValue("hasLight", true);
			}

			modelRenderer->GetModel().GetShader().Use();
			modelRenderer->GetModel().GetShader().SetValue("model", transform->GetModelMatrix());
			modelRenderer->GetModel().GetShader().SetValue("view", cam->GetViewMatrix());

			if (light != nullptr)
				modelRenderer->GetModel().GetShader().SetValue("lightPos", light->Position());

			modelRenderer->GetModel().GetShader().SetValue("viewPos", camTransform->Position());
			modelRenderer->GetModel().GetShader().SetValue("lightColor", Engine::Maths::Vector3(1.0f, 1.0f, 1.0f));

			for (Engine::Graphics::Mesh const& mesh : modelRenderer->GetModel().GetMeshes())
			{
				unsigned int texUnitIndex = 0;

				if (mesh.Mat != nullptr)
				{
					modelRenderer->GetModel().GetShader().SetValue("material.ambient", mesh.Mat->Ambient);
					modelRenderer->GetModel().GetShader().SetValue("material.specular", mesh.Mat->Specular);
					modelRenderer->GetModel().GetShader().SetValue("material.diffuse", mesh.Mat->Diffuse);
					modelRenderer->GetModel().GetShader().SetValue("material.illumType", static_cast<unsigned int>(mesh.Mat->Illumination));
					modelRenderer->GetModel().GetShader().SetValue("material.opacity", mesh.Mat->Opacity);

					if (mesh.Mat->AmbientTexture != nullptr)
					{
						glActiveTexture(GL_TEXTURE0 + texUnitIndex);
						glBindTexture(GL_TEXTURE_2D, mesh.Mat->AmbientTexture->ID);
						modelRenderer->GetModel().GetShader().SetValue("ambientTexture", texUnitIndex++);
						modelRenderer->GetModel().GetShader().SetValue("hasAmbientTex", 1);
					}
					else
					{
						modelRenderer->GetModel().GetShader().SetValue("hasAmbientTex", 0);
					}

					if (mesh.Mat->DiffuseTexture != nullptr)
					{
						glActiveTexture(GL_TEXTURE0 + texUnitIndex);
						glBindTexture(GL_TEXTURE_2D, mesh.Mat->DiffuseTexture->ID);
						modelRenderer->GetModel().GetShader().SetValue("diffuseTexture", texUnitIndex++);
						modelRenderer->GetModel().GetShader().SetValue("hasDiffuseTex", 1);
					}
					else
					{
						modelRenderer->GetModel().GetShader().SetValue("hasDiffuseTex", 0);
					}

					if (mesh.Mat->SpecularTexture != nullptr)
					{
						glActiveTexture(GL_TEXTURE0 + texUnitIndex);
						glBindTexture(GL_TEXTURE_2D, mesh.Mat->SpecularTexture->ID);
						modelRenderer->GetModel().GetShader().SetValue("specularTexture", texUnitIndex++);
						modelRenderer->GetModel().GetShader().SetValue("hasSpecularTex", 1);
					}
					else
					{
						modelRenderer->GetModel().GetShader().SetValue("hasSpecularTex", 0);
					}
				}
				else
				{
					std::cout << "Material not found for mesh <" << mesh.GroupName << ">" << std::endl;
				}

				glBindVertexArray(mesh.ID);
				glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.Vertices.size()));
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindVertexArray(0);
			}
		}
	}
}