#include "Vector3.h"

using namespace Engine::Maths;

Vector3 Engine::Maths::operator+(Vector3 const& src, float toAdd)
{
	return Vector3(src.x + toAdd, src.y + toAdd, src.z + toAdd);
}

Vector3 Engine::Maths::operator+(Vector3 const& src, int toAdd)
{
	return Vector3(src.x + toAdd, src.y + toAdd, src.z + toAdd);
}

Vector3 Engine::Maths::operator+(Vector3 const& src, double toAdd)
{
	return Vector3(src.x + static_cast<float>(toAdd), src.y + static_cast<float>(toAdd), src.z + static_cast<float>(toAdd));
}

Vector3 Engine::Maths::operator+(Vector3 const& src, Vector3 const& toAdd)
{
	return Vector3(src.x + toAdd.x, src.y + toAdd.y, src.z + toAdd.z);
}

Vector3 Engine::Maths::operator-(Vector3 const& src, float toSub)
{
	return Vector3(src.x - toSub, src.y - toSub, src.z - toSub);
}

Vector3 Engine::Maths::operator-(Vector3 const& src, int toSub)
{
	return Vector3(src.x - toSub, src.y - toSub, src.z - toSub);
}

Vector3 Engine::Maths::operator-(Vector3 const& src, double toSub)
{
	return Vector3(src.x - static_cast<float>(toSub), src.y - static_cast<float>(toSub), src.z - static_cast<float>(toSub));
}

Vector3 Engine::Maths::operator-(Vector3 const& src, Vector3 const& toSub)
{
	return Vector3(src.x - toSub.x, src.y - toSub.y, src.z - toSub.z);
}

Vector3 Engine::Maths::operator*(Vector3 const& src, float toMult)
{
	return Vector3(src.x * toMult, src.y * toMult, src.z * toMult);
}

Vector3 Engine::Maths::operator*(Vector3 const& src, int toMult)
{
	return Vector3(src.x * toMult, src.y * toMult, src.z * toMult);
}

Vector3 Engine::Maths::operator*(Vector3 const& src, double toMult)
{
	return Vector3(src.x * static_cast<float>(toMult), src.y * static_cast<float>(toMult), src.z * static_cast<float>(toMult));
}

Vector3 Engine::Maths::operator*(Vector3 const& src, Vector3 const& toMult)
{
	return Vector3(src.x * toMult.x, src.y * toMult.y, src.z * toMult.z);
}

Vector3 Engine::Maths::operator/(Vector3 const& src, float toDiv)
{
	return Vector3(src.x / toDiv, src.y / toDiv, src.z / toDiv);
}

Vector3 Engine::Maths::operator/(Vector3 const& src, int toDiv)
{
	return Vector3(src.x / toDiv, src.y / toDiv, src.z / toDiv);
}

Vector3 Engine::Maths::operator/(Vector3 const& src, double toDiv)
{
	return Vector3(src.x / static_cast<float>(toDiv), src.y / static_cast<float>(toDiv), src.z / static_cast<float>(toDiv));
}

Vector3 Engine::Maths::operator/(Vector3 const& src, Vector3 const& toDiv)
{
	return Vector3(src.x / toDiv.x, src.y / toDiv.y, src.z / toDiv.z);
}

std::ostream& Engine::Maths::operator<<(std::ostream& out, Vector3 const& toDisplay)
{
	out << toDisplay.x << "," << toDisplay.y << "," << toDisplay.z;
	return out;
}