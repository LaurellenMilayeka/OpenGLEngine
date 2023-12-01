#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Transform.h"
#include "ModelRenderer.h"
#include "Camera.h"
#include "Controller.h"

namespace Engine
{
	namespace Managers
	{
		class EntityManager;
	};

	namespace Entity
	{

		struct Entity
		{
			friend class Engine::Managers::EntityManager;

		private:

			std::vector<Engine::Components::IComponent*> _mComponents;

			std::string _ID;
			std::uint32_t _ComponentsFlags;

			Entity(std::string id)
			{
				IsEnabled = true;
				_ID = id;
				_ComponentsFlags = 0;

				_mComponents.reserve(32);

				_ComponentsFlags |= static_cast<std::uint32_t>(Components::ComponentsID::Transform);
				_mComponents.emplace_back(new Components::Transform(this));
			}

		public:

			bool IsEnabled;

			std::string ID() const
			{
				return _ID;
			}

			template<typename T>
			void AddComponent()
			{
				static_assert(std::is_base_of<Engine::Components::IComponent, T>::value, "Error : Cannot add component that is not part of IComponent class");
				throw new std::exception("Component cannot be added");
			}

			template<>
			void AddComponent<Components::ModelRenderer>()
			{
				_ComponentsFlags |= static_cast<std::uint32_t>(Components::ComponentsID::ModelRenderer);
				_mComponents.emplace_back(new Components::ModelRenderer(this));
			}

			template<>
			void AddComponent<Components::Controller>()
			{
				_ComponentsFlags |= static_cast<std::uint32_t>(Components::ComponentsID::Controller);
				_mComponents.emplace_back(new Components::Controller(this));
			}

			template<>
			void AddComponent<Components::Camera>()
			{
				_ComponentsFlags |= static_cast<std::uint32_t>(Components::ComponentsID::Camera);
				_mComponents.emplace_back(new Components::Camera(this));
			}

			template<typename T>
			T* GetComponent() const
			{
				for (Engine::Components::IComponent* item : _mComponents)
				{
					if (dynamic_cast<T*>(item) != nullptr)
					{
						return dynamic_cast<T*>(item);
					}
				}
				return nullptr;
			}

			bool HasComponent(Components::ComponentsID componentId) const
			{
				return ((_ComponentsFlags & (std::uint32_t)componentId) == (std::uint32_t)componentId);
			}

			void ListComponents() const
			{
				std::cout << "Components : " << std::endl;
				for (auto& item : _mComponents)
				{
					std::cout << "    " << item->Name() << std::endl;;
				}
			}

			void SetupDebugWindow() const
			{
				for (Engine::Components::IComponent* component : _mComponents)
				{
					if (ImGui::TreeNode(component->Name().c_str()))
					{
						component->SetupDebugWindow();
						ImGui::TreePop();
					}
				}
			}

#ifdef _DEBUG

			std::vector<Engine::Components::IComponent*> GetComponents()
			{
				return _mComponents;
			}

#endif

		};
	}
}