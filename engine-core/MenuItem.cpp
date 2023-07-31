#include "MenuItem.h"

Engine::GUI::MenuItem::MenuItem(std::string const& label, std::string const& icon)
{
	_mIcon = icon;
	_mItemName = label;
}
