#pragma once

#include "MenuItem.h"

#include <map>
#include <string>

namespace Engine
{
	namespace GUI
	{
		class TopMenuBar
		{
		private:

			std::map<unsigned int, MenuItem> _mItems;

		public:

			TopMenuBar();

			void AddItem(std::string const& label, std::string const& icon = "");
		};
	}
}


