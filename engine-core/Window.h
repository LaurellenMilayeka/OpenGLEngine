#pragma once

#include <cstdint>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
	namespace Graphics
	{
		class Window
		{
		private:

			GLFWwindow* _mWindow;
			std::string _mTitle;
			static std::uint16_t _mHeight, _mWidth;

		public:

			Window(std::uint16_t width, std::uint16_t height, std::string const& title);
			~Window() = default; /// TODO : Masterclass dtor

			static void ModifiedFramebufferSizeCallback(GLFWwindow* window, int width, int height);
			void SetFramebufferSize(std::uint16_t width, std::uint16_t height);

			bool Create ();
			void Destroy();
			void Update ();

			float Height() const;
			float Width() const;

			void ShowCursor(bool showCursor);

			GLFWwindow* operator*()
			{
				return _mWindow;
			}
		};
	}
}

