#include "Renderer.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GL/glew.h>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <imgui.h>

using namespace Engine::Systems;

void Engine::Systems::Renderer::Update3DScene()
{
	std::vector<Engine::Entity::Entity*> entityList = Managers::EntityManager::_mEntityPool;
	Engine::Components::Camera* cam = Managers::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Camera>();
	Engine::Components::Transform* camTransform = Managers::EntityManager::Get("MainCamera")->GetComponent<Engine::Components::Transform>();

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1600) / static_cast<float>(900), 0.1f, 10000.0f);

	assert(cam != nullptr);

	for (Engine::Entity::Entity* item : entityList)
	{
		if (item->HasComponent(Engine::Components::ComponentsID::Transform)
			&& item->HasComponent(Engine::Components::ComponentsID::ModelRenderer)
			&& item->IsEnabled)
		{
			Components::Transform* transform = item->GetComponent<Components::Transform>();
			Components::ModelRenderer* modelRenderer = item->GetComponent<Components::ModelRenderer>();

			if (modelRenderer->GetModel().IsEnabled)
			{
				modelRenderer->GetModel().GetShader().Use();
				modelRenderer->GetModel().GetShader().SetValue("projection", projection);
				modelRenderer->GetModel().GetShader().SetValue("model", transform->GetModelMatrix());
				modelRenderer->GetModel().GetShader().SetValue("view", cam->GetViewMatrix());
				modelRenderer->GetModel().GetShader().SetValue("viewPos", camTransform->Position());

				for (Engine::Graphics::Mesh& mesh : modelRenderer->GetModel().GetMeshes())
				{
					unsigned int texUnitIndex = 0;

					modelRenderer->GetModel().GetShader().SetValue("has3DTexture", mesh.Is3DTexture);
					if (mesh.Mat != nullptr)
					{
						modelRenderer->GetModel().GetShader().SetValue("ambientColor", mesh.Mat->Ambient);

						if (mesh.Mat->AmbientTexture)
						{
							glActiveTexture(GL_TEXTURE0);
							glBindTexture((mesh.Mat->DiffuseTexture->Type == Engine::Graphics::TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D, mesh.Mat->DiffuseTexture->ID);
							
							modelRenderer->GetModel().GetShader().SetValue("hasTexture", true); 
							modelRenderer->GetModel().GetShader().SetValue("overallTexture", 0);
						}
						else
						{
							modelRenderer->GetModel().GetShader().SetValue("hasTexture", false);
						}
					}
					else
					{
						std::cout << "Material not found for mesh <" << mesh.GroupName << ">" << std::endl;
					}

					if (mesh.IsEnabled)
					{
						glBindVertexArray(mesh.ID);
						if (mesh.EBOQuads != 0 || mesh.EBOTriangles != 0)
						{
							if (mesh.EBOTriangles != 0)
							{
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBOTriangles);
								glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.Indices[Engine::Graphics::PolyType::TRIANGLES].size()), GL_UNSIGNED_INT, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							}

							if (mesh.EBOQuads != 0)
							{
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBOQuads);
								glDrawElements(GL_QUADS, static_cast<GLsizei>(mesh.Indices[Engine::Graphics::PolyType::QUADS].size()), GL_UNSIGNED_INT, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
							}
						}
						else
						{
							glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.Vertices.size()));
						}

						if (mesh.Mat->DiffuseTexture)
							glBindTexture((mesh.Mat->DiffuseTexture->Type == Engine::Graphics::TextureType::TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_3D, 0);

						glBindVertexArray(0);
					}
				}
			}
		}
	}
}

void Engine::Systems::Renderer::UpdateEngineGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::Update()
{
	Update3DScene();
	UpdateEngineGUI();
}