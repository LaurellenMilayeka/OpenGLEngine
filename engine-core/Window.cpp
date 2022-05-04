#include "Window.h"

#include <cassert>

using namespace Engine::Graphics;

std::uint16_t Window::_mHeight, Window::_mWidth;

void Window::ModifiedFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	_mWidth = width;
	_mHeight = height;
}

Window::Window(std::uint16_t width, std::uint16_t height, std::string const& title)
{
	_mWidth  = width;
	_mHeight = height;
	_mTitle = title;
}

void Window::SetFramebufferSize(std::uint16_t width, std::uint16_t height)
{
	_mWidth = width;
	_mHeight = height;
}

bool Window::Create()
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	_mWindow = glfwCreateWindow(_mWidth, _mHeight, _mTitle.c_str(), nullptr, nullptr);
	assert(nullptr != _mWindow);
	glfwMakeContextCurrent(_mWindow);
	
	glfwSetWindowSizeCallback(_mWindow, ModifiedFramebufferSizeCallback);
	if (glewInit() != GLEW_OK)
	{
		return false;
	}
	return true;
}

void Window::Destroy()
{
	glfwDestroyWindow(_mWindow);
	_mWindow = nullptr;
}

void Window::Update()
{
	glfwSwapBuffers(_mWindow);
}

float Window::Height() const
{
	return _mHeight;
}

float Window::Width() const
{
	return _mWidth;
}