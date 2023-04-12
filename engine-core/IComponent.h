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

			ComponentsID _mComponentID;

			IComponent(std::string const& name, ComponentsID componentId, Engine::Entity::Entity* parent)
			{
				_mName = name;
				_mParent = parent;
				_mComponentID = componentId;
			}

		public:

			virtual ~IComponent() = default;

#ifdef _DEBUG
			virtual void SetupDebugWindow() { }
#endif

			std::string const& Name() const
			{
				return _mName;
			}

			ComponentsID GetType() const
			{
				return _mComponentID;
			}
		};
	}
}