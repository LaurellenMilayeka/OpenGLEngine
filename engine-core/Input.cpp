#include "Input.h"

#include <iostream>
#include <glm.hpp>

#include "Helpers.h"

using namespace Engine::InputManager;

std::map<Key, bool> Input::_keyState = {
	{Key::Z, false},
	{Key::Q, false},
	{Key::S, false},
	{Key::D, false},
	{Key::ESC, false},
	{Key::ALT, false}
};

Engine::Maths::Vector2 Input::_mMousePosition;
Engine::Maths::Vector2 Input::_mLastMousePosition;

void Input::MousePosition(GLFWwindow* window, double xpos, double ypos)
{
	_mMousePosition.x = static_cast<float>(xpos);
	_mMousePosition.y = static_cast<float>(ypos);
}

void Input::LastMousePosition(Engine::Maths::Vector2 const& lastMousePosition)
{
	_mLastMousePosition.x = lastMousePosition.x;
	_mLastMousePosition.y = lastMousePosition.y;
}

void Input::Initialize(float xpos, float ypos)
{
	_mMousePosition.x = xpos;
	_mMousePosition.y = ypos;

	_mLastMousePosition.x = xpos;
	_mLastMousePosition.y = ypos;
}

void Input::Update(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "Scancode : " << std::to_string(scancode) << std::endl;
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			_keyState[Key::ESC] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
			break;
		default:
			switch (glfwGetKeyScancode(key))
			{
			case KEY_SCANCODE_W:
				_keyState[Key::Z] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
				break;
			case KEY_SCANCODE_A:
				_keyState[Key::Q] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
				break;
			case KEY_SCANCODE_S:
				_keyState[Key::S] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
				break;
			case KEY_SCANCODE_D:
				_keyState[Key::D] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
				break;
			case KEY_SCANCODE_ALT:
				_keyState[Key::ALT] = (action == GLFW_PRESS || action == GLFW_REPEAT) ? true : false;
				break;
			}
			break;
	}
	return;
}

bool Input::GetKey(Key toGet)
{
	static std::map<Key, bool> kbContext;
	bool retval = false;

	if (_keyState.find(toGet)->second && ! kbContext.find(toGet)->second)
	{
		retval = true;
	}
	kbContext = _keyState;
	return retval;
}

bool Input::GetKeyDown(Key toGet)
{
	return _keyState.at(toGet);
}

Engine::Maths::Vector2 const& Input::MousePosition()
{
	return _mMousePosition;
}

Engine::Maths::Vector2 const& Input::LastMousePosition()
{
	return _mLastMousePosition;
}
