#pragma once

namespace Engine
{
	namespace Systems
	{
		class Renderer
		{
		private:

		public:

			void Update();

#ifdef _DEBUG
			void SetupDebugRendererWindow();
#endif
		};
	}
}

