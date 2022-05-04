#include "Vector2.h"

using namespace Engine::Maths;

Vector2 Engine::Maths::operator+(Vector2 const& src, float toAdd)
{
	return Vector2(src.x + toAdd, src.y + toAdd);
}

Vector2 Engine::Maths::operator+(Vector2 const& src, int toAdd)
{
	return Vector2(src.x + toAdd, src.y + toAdd);
}

Vector2 Engine::Maths::operator+(Vector2 const& src, double toAdd)
{
	return Vector2(src.x + static_cast<float>(toAdd), src.y + static_cast<float>(toAdd));
}

Vector2 Engine::Maths::operator+(Vector2 const& src, Vector2 const& toAdd)
{
	return Vector2(src.x + toAdd.x, src.y + toAdd.y);
}

Vector2 Engine::Maths::operator-(Vector2 const& src, float toAdd)
{
	return Vector2(src.x - toAdd, src.y - toAdd);
}

Vector2 Engine::Maths::operator-(Vector2 const& src, int toAdd)
{
	return Vector2(src.x - toAdd, src.y - toAdd);
}

Vector2 Engine::Maths::operator-(Vector2 const& src, double toAdd)
{
	return Vector2(src.x - static_cast<float>(toAdd), src.y - static_cast<float>(toAdd));
}

Vector2 Engine::Maths::operator-(Vector2 const& src, Vector2 const& toAdd)
{
	return Vector2(src.x - toAdd.x, src.y - toAdd.y);
}

Vector2 Engine::Maths::operator*(Vector2 const& src, float toAdd)
{
	return Vector2(src.x * toAdd, src.y * toAdd);
}

Vector2 Engine::Maths::operator*(Vector2 const& src, int toAdd)
{
	return Vector2(src.x * toAdd, src.y * toAdd);
}

Vector2 Engine::Maths::operator*(Vector2 const& src, double toAdd)
{
	return Vector2(src.x * static_cast<float>(toAdd), src.y * static_cast<float>(toAdd));
}

Vector2 Engine::Maths::operator*(Vector2 const& src, Vector2 const& toAdd)
{
	return Vector2(src.x * toAdd.x, src.y * toAdd.y);
}

Vector2 Engine::Maths::operator/(Vector2 const& src, float toAdd)
{
	return Vector2(src.x / toAdd, src.y / toAdd);
}

Vector2 Engine::Maths::operator/(Vector2 const& src, int toAdd)
{
	return Vector2(src.x / toAdd, src.y / toAdd);
}

Vector2 Engine::Maths::operator/(Vector2 const& src, double toAdd)
{
	return Vector2(src.x / static_cast<float>(toAdd), src.y / static_cast<float>(toAdd));
}

Vector2 Engine::Maths::operator/(Vector2 const& src, Vector2 const& toAdd)
{
	return Vector2(src.x / toAdd.x, src.y / toAdd.y);
}