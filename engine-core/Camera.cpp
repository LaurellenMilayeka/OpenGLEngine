#include "Camera.h"
#include "Entity.h"
#include "Input.h"
#include "Helpers.h"
#include "Time.h"

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace Engine::Components;

Camera::Camera(Engine::Entity::Entity* parent) : IComponent("Camera", ComponentsID::Camera, parent)
{
	_mYaw = -90.0f;
	_mPitch = 0.0f;
	_mFront = Engine::Maths::Vector3::Zero();
	_mRight = Engine::Maths::Vector3::Zero();
	_mUp = Engine::Maths::Vector3::Zero();
	_mWorldUp = Engine::Maths::Vector3::Up();
	_mDisableCameraMovements = false;
	UpdateCameraVectors();
}

void Camera::DisableCameraMovements()
{
	_mDisableCameraMovements = true;
}

void Camera::EnableCameraMovements()
{
	_mDisableCameraMovements = false;
}

Engine::Maths::Matrix4 const Camera::GetViewMatrix()
{
	Transform* transform = _mParent->GetComponent<Transform>();
	
	UpdateCameraVectors();
	glm::vec3 pos = { transform->Position().x, transform->Position().y, transform->Position().z };
	glm::vec3 front = { _mFront.x, _mFront.y, _mFront.z };
	glm::vec3 up = { _mUp.x, _mUp.y, _mUp.z };
	glm::mat4 view = glm::lookAt(pos, pos - front, up);
	return glm::value_ptr(view);
}

void Camera::UpdateCameraVectors()
{
	Transform* transform = _mParent->GetComponent<Transform>();
	Engine::Maths::Vector2 mousePos = Engine::InputManager::Input::MousePosition();
	Engine::Maths::Vector2 lastMousePos = Engine::InputManager::Input::LastMousePosition();

	float xoffset = mousePos.x - lastMousePos.x;
	float yoffset = lastMousePos.y - mousePos.y;

	Engine::InputManager::Input::LastMousePosition(mousePos);

	if (!_mDisableCameraMovements)
	{
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		_mYaw += xoffset;
		_mPitch += yoffset;

		if (_mPitch > 89.0f)
			_mPitch = 89.0f;
		else if (_mPitch < -89.0f)
			_mPitch = -89.0f;

		_mDirection.x = -cos(Helpers::DegreeToRad(_mPitch)) * cos(Helpers::DegreeToRad(_mYaw));
		_mDirection.y = -sin(Helpers::DegreeToRad(_mPitch));
		_mDirection.z = -cos(Helpers::DegreeToRad(_mPitch)) * sin(Helpers::DegreeToRad(_mYaw));

		_mFront = Engine::Maths::Vector3::Normalize(_mDirection);
		_mRight = Engine::Maths::Vector3::Normalize(Engine::Maths::Vector3::Cross(_mFront, _mWorldUp));
		_mUp = Engine::Maths::Vector3::Normalize(Engine::Maths::Vector3::Cross(_mRight, _mFront));

		Engine::Maths::Vector3 currentPos = Engine::Maths::Vector3::Zero();

		_mSpeed = 35.0f;
		if (Engine::InputManager::Input::GetKeyDown(Engine::InputManager::Key::LSHIFT))
		{
			_mSpeed *= 3;
		}

		if (Engine::InputManager::Input::GetKeyDown(Engine::InputManager::Key::Z))
		{
			currentPos -= _mFront * _mSpeed * Engine::Misc::Time::DeltaTime();
		}
		if (Engine::InputManager::Input::GetKeyDown(Engine::InputManager::Key::S))
		{
			currentPos += _mFront * _mSpeed * Engine::Misc::Time::DeltaTime();
		}
		if (Engine::InputManager::Input::GetKeyDown(Engine::InputManager::Key::Q))
		{
			currentPos -= Engine::Maths::Vector3::Normalize(Engine::Maths::Vector3::Cross(_mUp, _mFront)) * _mSpeed * Engine::Misc::Time::DeltaTime();
		}
		if (Engine::InputManager::Input::GetKeyDown(Engine::InputManager::Key::D))
		{
			currentPos += Engine::Maths::Vector3::Normalize(Engine::Maths::Vector3::Cross(_mUp, _mFront)) * _mSpeed * Engine::Misc::Time::DeltaTime();
		}
		transform->Translate(currentPos);
	}

}
