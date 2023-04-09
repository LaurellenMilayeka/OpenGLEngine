#include "Renderer.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include <GL/glew.h>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace Engine::Systems;

void Renderer::Update()
{
	std::vector<Engine::Entity::Entity*> entityList = Engine::Entity::EntityManager::_mEntityPool;
	Engine::Components::Camera* cam = Entity::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Camera>();
	Engine::Components::Transform* camTransform = Entity::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Transform>();

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.1f, 1000.0f);

	assert(cam != nullptr);

	for (Engine::Entity::Entity* item : entityList)
	{
		if (item->HasComponent(Engine::Components::ComponentsID::Transform)
			&& item->HasComponent(Engine::Components::ComponentsID::ModelRenderer))
		{
			Components::Transform* transform = item->GetComponent<Components::Transform>();
			Components::ModelRenderer* modelRenderer = item->GetComponent<Components::ModelRenderer>();

			modelRenderer->GetModel().GetShader().Use();
			modelRenderer->GetModel().GetShader().SetValue("projection", projection);
			modelRenderer->GetModel().GetShader().SetValue("model", transform->GetModelMatrix());
			modelRenderer->GetModel().GetShader().SetValue("view", cam->GetViewMatrix());
			modelRenderer->GetModel().GetShader().SetValue("viewPos", camTransform->Position());

			for (Engine::Graphics::Mesh const& mesh : modelRenderer->GetModel().GetMeshes())
			{
				unsigned int texUnitIndex = 0;

				modelRenderer->GetModel().GetShader().SetValue("has3DTexture", mesh.Is3DTexture);
				if (mesh.Mat != nullptr)
				{
					modelRenderer->GetModel().GetShader().SetValue("ambientColor", mesh.Mat->Ambient);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture((mesh.Mat->DiffuseTexture->Type == Engine::Graphics::TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D, mesh.Mat->DiffuseTexture->ID);
					modelRenderer->GetModel().GetShader().SetValue("overallTexture", 0);
				}
				else
				{
					std::cout << "Material not found for mesh <" << mesh.GroupName << ">" << std::endl;
				}

				if (mesh.IsEnabled)
				{
					glBindVertexArray(mesh.ID);
					if (mesh.Indices.size() > 0)
					{
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
						glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.Indices.size()), GL_UNSIGNED_INT, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					}
					else
					{
						glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.Vertices.size()));
					}

					if (mesh.Mat != nullptr)
						glBindTexture((mesh.Mat->DiffuseTexture->Type == Engine::Graphics::TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0);

					glBindVertexArray(0);
				}
			}
		}
	}
}