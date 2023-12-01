#pragma once

#include <vector>
#include <string>

#include "Renderer.h"
#include "InputManager.h"

namespace Engine
{
	namespace Entity
	{
		struct Entity;
	};

	namespace Managers
	{
		class EntityManager
		{

			friend class Engine::Systems::Renderer;
			friend class Engine::Systems::InputManager;

		private:

			static std::vector<Engine::Entity::Entity*> _mEntityPool;

		public:

			static Engine::Entity::Entity* Create(std::string const& entityName);
			static void Delete(Engine::Entity::Entity const* toDelete);

			static Engine::Entity::Entity const* Get(std::string const& id);
			static std::vector<Engine::Entity::Entity*> const& GetAll();

		};
	}
}

