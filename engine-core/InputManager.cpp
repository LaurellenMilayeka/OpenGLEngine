#include "InputManager.h"

#include <vector>

#include "EntityManager.h"

using namespace Engine::Systems;

void InputManager::Update()
{
	std::vector<Engine::Entity::Entity*> entityList = Engine::Entity::EntityManager::_mEntityPool;

	for (Engine::Entity::Entity* ent : entityList)
	{
		if (ent->HasComponent(Engine::Components::ComponentsID::Transform))
		{

		}
	}
}
