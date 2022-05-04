#pragma once

#include <string>

namespace Engine
{

	namespace Entity
	{
		struct Entity;
	}

	namespace Components
	{
		enum class ComponentsID
		{
			Transform = 0x01,
			ModelRenderer = 0x02,
			Controller = 0x04,
			Camera = 0x08
		};

		class IComponent
		{
		protected:

			std::string _mName;
			Engine::Entity::Entity* _mParent;

			IComponent(std::string const& name, Engine::Entity::Entity* parent)
			{
				_mName = name;
				_mParent = parent;
			}

		public:

			virtual ~IComponent() = default;

			std::string Name()
			{
				return _mName;
			}
		};
	}
}