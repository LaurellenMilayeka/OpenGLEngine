#pragma once

#include <imgui.h>

#include "IComponent.h"

namespace Engine
{
	namespace Components
	{
		class Controller : public IComponent
		{
		public:
			Controller(Engine::Entity::Entity* parent);
			virtual ~Controller() = default;

			virtual void SetupDebugWindow() override
			{
				ImGui::Text("Controller");
			}

		};
	}
}

