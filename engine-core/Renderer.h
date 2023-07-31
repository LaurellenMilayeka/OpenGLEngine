#pragma once

namespace Engine
{
	namespace Systems
	{
		class Renderer
		{
		private:

			void Update3DScene();
			void UpdateEngineGUI();

		public:

			void Update();

		};
	}
}

