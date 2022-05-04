#pragma once

#include "Vector2.h"
#include "Vector3.h"

#include <map>
#include <GLFW/glfw3.h>

namespace Engine
{
	namespace InputManager
	{
		
#define KEY_SCANCODE_W 0x11
#define KEY_SCANCODE_A 0x1E
#define KEY_SCANCODE_S 0x1F
#define KEY_SCANCODE_D 0x20

		enum class Key : std::uint32_t {
			Z,
			Q,
			S,
			D,
			ESC,
			UNKNOWN
		};

		class Input
		{
		private:

			static std::map<Key, bool> _keyState;
			static Engine::Maths::Vector2 _mMousePosition;
			static Engine::Maths::Vector2 _mLastMousePosition;

		public:

			static void MousePosition(GLFWwindow* window, double xpos, double ypos);
			static void LastMousePosition(Engine::Maths::Vector2 const& lastMousePosition);
			static void Initialize(float xpos, float ypos);
			static void Update(GLFWwindow* window, int key, int scancode, int action, int mods);

			static bool GetKey(Key toGet);
			static bool GetKeyDown(Key toGet);
			static Engine::Maths::Vector2 const& MousePosition();
			static Engine::Maths::Vector2 const& LastMousePosition();

		};
	}
}

