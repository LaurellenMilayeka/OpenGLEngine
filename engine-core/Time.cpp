#include "Time.h"

#include <GLFW/glfw3.h>

using namespace Engine::Misc;

double Time::_mCurrentFrame = 0;
double Time::_mLastFrame    = 0;
double Time::_mDeltaTime    = 0;

void Time::Update()
{
    _mCurrentFrame = glfwGetTime();
    _mDeltaTime = _mCurrentFrame - _mLastFrame;
    _mLastFrame = _mCurrentFrame;
}

float Time::DeltaTime()
{
    return static_cast<float>(_mDeltaTime);
}
