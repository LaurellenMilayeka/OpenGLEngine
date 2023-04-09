#pragma once

#include "IComponent.h"
#include "Matrix4.h"
#include "Vector2.h"
#include "Renderer.h"

namespace Engine
{
	namespace Components
	{
		class Camera : public IComponent
		{
			friend class Engine::Systems::Renderer;

		private:

			Engine::Maths::Vector3 _mDirection;
			Engine::Maths::Matrix4 _mViewMatrix;

			Engine::Maths::Vector3 _mFront;
			Engine::Maths::Vector3 _mUp;
			Engine::Maths::Vector3 _mRight;
			Engine::Maths::Vector3 _mWorldUp;

			bool _mDisableCameraMovements;

			float _mYaw;
			float _mPitch;

			Engine::Maths::Matrix4 const GetViewMatrix();
			void UpdateCameraVectors();

		public:

			Camera(Engine::Entity::Entity* parent);
			virtual ~Camera() = default;

			void DisableCameraMovements();
			void EnableCameraMovements();

		};
	}
}