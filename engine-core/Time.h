#pragma once

namespace Engine
{
	namespace Misc
	{
		class Time
		{
		private:
			static double _mCurrentFrame;
			static double _mLastFrame;
			static double _mDeltaTime;

		public:

			static void Update();

			static float DeltaTime();
		};
	};
}

