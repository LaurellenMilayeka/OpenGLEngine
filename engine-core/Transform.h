#pragma once

#include "IComponent.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Renderer.h"
#include "Camera.h"

namespace Engine
{
	namespace Components
	{
		class Transform : public IComponent
		{
			friend class Engine::Systems::Renderer;
		
		private:

			Engine::Maths::Matrix4 _mTranslationMatrix;
			Engine::Maths::Matrix4 _mScaleMatrix;
			Engine::Maths::Matrix4 _mRotationMatrix;

			Engine::Maths::Matrix4 GetModelMatrix() const;

			void setRotation(float deg, Engine::Maths::Vector3 const& newRotation);

		public:

			Transform(Engine::Entity::Entity* parent);
			virtual ~Transform();

			Engine::Maths::Vector3 Position() const;
			Engine::Maths::Vector3 Scale() const;
			Engine::Maths::Vector3 Rotation() const;

			void Translate(Engine::Maths::Vector3 const& newPoint);
			void Rotate(float deg, Engine::Maths::Vector3 const& newRotation);
			void Scale(Engine::Maths::Vector3 const& newScale);
		};
	}
}

