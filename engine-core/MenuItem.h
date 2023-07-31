#pragma once

#include <string>

namespace Engine
{
	namespace GUI
	{
		class MenuItem
		{
		private:
			std::string _mIcon;
			std::string _mItemName;

		public:

			MenuItem(std::string const& label, std::string const& icon);

		};
	}
}

