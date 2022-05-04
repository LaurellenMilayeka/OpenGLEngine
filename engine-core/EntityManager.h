#pragma once

#include <vector>
#include <string>

#include "Entity.h"
#include "Renderer.h"
#include "InputManager.h"

namespace Engine
{
	namespace Entity
	{
		class EntityManager
		{

			friend class Engine::Systems::Renderer;
			friend class Engine::Systems::InputManager;

		private:

			static std::vector<Entity*> _mEntityPool;

		public:

			static Entity* Create(std::string const& entityName);
			static void Delete(Entity const& toDelete);

			static Entity const* Get(std::string const& id);

		};
	}
}

