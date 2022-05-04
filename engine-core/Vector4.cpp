#include "Vector4.h"

using namespace Engine::Maths;

Vector4 Engine::Maths::operator+(Vector4 const& src, float toAdd)
{
	return Vector4(src.x + toAdd, src.y + toAdd, src.z + toAdd, src.w + toAdd);
}

Vector4 Engine::Maths::operator+(Vector4 const& src, int toAdd)
{
	return Vector4(src.x + toAdd, src.y + toAdd, src.z + toAdd, src.w + toAdd);
}

Vector4 Engine::Maths::operator+(Vector4 const& src, double toAdd)
{
	return Vector4(src.x + static_cast<float>(toAdd), src.y + static_cast<float>(toAdd), src.z + static_cast<float>(toAdd), src.w + static_cast<float>(toAdd));
}

Vector4 Engine::Maths::operator+(Vector4 const& src, Vector4 const& toAdd)
{
	return Vector4(src.x + toAdd.x, src.y + toAdd.y, src.z + toAdd.z, src.w + toAdd.w);
}

Vector4 Engine::Maths::operator-(Vector4 const& src, float toSub)
{
	return Vector4(src.x - toSub, src.y - toSub, src.z - toSub, src.w - toSub);
}

Vector4 Engine::Maths::operator-(Vector4 const& src, int toSub)
{
	return Vector4(src.x - toSub, src.y - toSub, src.z - toSub, src.w - toSub);
}

Vector4 Engine::Maths::operator-(Vector4 const& src, double toSub)
{
	return Vector4(src.x - static_cast<float>(toSub), src.y - static_cast<float>(toSub), src.z - static_cast<float>(toSub), src.w - static_cast<float>(toSub));
}

Vector4 Engine::Maths::operator-(Vector4 const& src, Vector4 const& toSub)
{
	return Vector4(src.x - toSub.x, src.y - toSub.y, src.z - toSub.z, src.w - toSub.w);
}

Vector4 Engine::Maths::operator*(Vector4 const& src, float toMult)
{
	return Vector4(src.x * toMult, src.y * toMult, src.z * toMult, src.w * toMult);
}

Vector4 Engine::Maths::operator*(Vector4 const& src, int toMult)
{
	return Vector4(src.x * toMult, src.y * toMult, src.z * toMult, src.w * toMult);
}

Vector4 Engine::Maths::operator*(Vector4 const& src, double toMult)
{
	return Vector4(src.x * static_cast<float>(toMult), src.y * static_cast<float>(toMult), src.z * static_cast<float>(toMult), src.w * static_cast<float>(toMult));
}

Vector4 Engine::Maths::operator*(Vector4 const& src, Vector4 const& toMult)
{
	return Vector4(src.x * toMult.x, src.y * toMult.y, src.z * toMult.z, src.w * toMult.w);
}

Vector4 Engine::Maths::operator/(Vector4 const& src, float toDiv)
{
	return Vector4(src.x / toDiv, src.y / toDiv, src.z / toDiv, src.w / toDiv);
}

Vector4 Engine::Maths::operator/(Vector4 const& src, int toDiv)
{
	return Vector4(src.x / toDiv, src.y / toDiv, src.z / toDiv, src.w / toDiv);
}

Vector4 Engine::Maths::operator/(Vector4 const& src, double toDiv)
{
	return Vector4(src.x / static_cast<float>(toDiv), src.y / static_cast<float>(toDiv), src.z / static_cast<float>(toDiv), src.w / static_cast<float>(toDiv));
}

Vector4 Engine::Maths::operator/(Vector4 const& src, Vector4 const& toDiv)
{
	return Vector4(src.x / toDiv.x, src.y / toDiv.y, src.z / toDiv.z, src.w / toDiv.w);
}