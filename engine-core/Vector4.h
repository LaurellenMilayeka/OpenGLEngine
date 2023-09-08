#pragma once

#include <math.h>
#include "Vector3.h"

namespace Engine
{
	namespace Maths
	{
		struct Vector4
		{
			float x;
			float y;
			float z;
			float w;

			Vector4()
			{
				x = 0;
				y = 0;
				z = 0;
				w = 0;
			}

			Vector4(float xyzw)
			{
				x = xyzw;
				y = xyzw;
				z = xyzw;
				w = xyzw;
			}

			Vector4(float x, float y, float z, float w)
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			Vector4(Vector3 const& vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
				this->w = 1.0f;
			}

			Vector4(Vector2 const& vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = 0.0f;
				this->w = 1.0f;
			}

			static Vector4 Zero()
			{
				return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			}

			static Vector4 Up()
			{
				return Vector4(0.0f, 1.0f, 0.0f, 0.0f);
			}

			static Vector4 Down()
			{
				return Vector4(0.0f, -1.0f, 0.0f, 0.0f);
			}

			static Vector4 Left()
			{
				return Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
			}

			static Vector4 Right()
			{
				return Vector4(1.0f, 0.0f, 0.0f, 0.0f);
			}

			void Negate()
			{
				this->x *= -1;
				this->y *= -1;
				this->z *= -1;
				this->w *= -1;
			}

			float Magnitude()
			{
				return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));
			}

			float Length()
			{
				return Magnitude();
			}

			float Dot(Vector4 const& toDot)
			{
				return (this->x * toDot.x) + (this->y * toDot.y) + (this->z * toDot.z) + (this->w * toDot.w);
			}

			Vector4 Cross(Vector4 const& toCross)
			{
				return Vector3
					((this->y * toCross.z) - (this->z * toCross.y),
					(this->z * toCross.x) - (this->x * toCross.z),
					(this->x * toCross.y) - (this->y * toCross.x));
			}

			static Vector4 Cross(Vector4 const& vec1, Vector4 const& vec2)
			{
				return Vector3
					((vec1.y * vec2.z) - (vec1.z * vec2.y),
					(vec1.z * vec2.x) - (vec1.x * vec2.z),
					(vec1.x * vec2.y) - (vec1.y * vec2.x));

			}

			void operator-()
			{
				this->x *= -1;
				this->y *= -1;
				this->z *= -1;
				this->w *= -1;
			}

			void operator+=(float toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
				this->z += toAdd;
				this->w += toAdd;
			}

			void operator+=(int toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
				this->z += toAdd;
				this->w += toAdd;
			}

			void operator+=(double toAdd)
			{
				this->x += static_cast<float>(toAdd);
				this->y += static_cast<float>(toAdd);
				this->z += static_cast<float>(toAdd);
				this->w += static_cast<float>(toAdd);
			}

			void operator+=(Vector4 const& toAdd)
			{
				this->x += toAdd.x;
				this->y += toAdd.y;
				this->z += toAdd.z;
				this->w += toAdd.w;
			}

			void operator-=(float toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
				this->z -= toSub;
				this->w -= toSub;
			}

			void operator-=(int toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
				this->z -= toSub;
				this->w -= toSub;
			}

			void operator-=(double toSub)
			{
				this->x -= static_cast<float>(toSub);
				this->y -= static_cast<float>(toSub);
				this->z -= static_cast<float>(toSub);
				this->w -= static_cast<float>(toSub);
			}

			void operator-=(Vector4 const& toSub)
			{
				this->x -= toSub.x;
				this->y -= toSub.y;
				this->z -= toSub.z;
				this->w -= toSub.w;
			}

			void operator*=(float toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
				this->z *= toMult;
				this->w *= toMult;
			}

			void operator*=(int toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
				this->z *= toMult;
				this->w *= toMult;
			}

			void operator*=(double toMult)
			{
				this->x *= static_cast<float>(toMult);
				this->y *= static_cast<float>(toMult);
				this->z *= static_cast<float>(toMult);
				this->w *= static_cast<float>(toMult);
			}

			void operator*=(Vector4 const& toMult)
			{
				this->x *= toMult.x;
				this->y *= toMult.y;
				this->z *= toMult.z;
				this->w *= toMult.w;
			}

			void operator/=(float toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
				this->z /= toDiv;
				this->w /= toDiv;
			}

			void operator/=(int toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
				this->z /= toDiv;
				this->w /= toDiv;
			}

			void operator/=(double toDiv)
			{
				this->x /= static_cast<float>(toDiv);
				this->y /= static_cast<float>(toDiv);
				this->z /= static_cast<float>(toDiv);
				this->w /= static_cast<float>(toDiv);
			}

			void operator/=(Vector4 const& toDiv)
			{
				this->x /= toDiv.x;
				this->y /= toDiv.y;
				this->z /= toDiv.z;
				this->w /= toDiv.w;
			}
		};

		Vector4 operator+(Vector4 const& src, float toAdd);
		Vector4 operator+(Vector4 const& src, int toAdd);
		Vector4 operator+(Vector4 const& src, double toAdd);
		Vector4 operator+(Vector4 const& src, Vector4 const& toAdd);
		Vector4 operator-(Vector4 const& src, float toSub);
		Vector4 operator-(Vector4 const& src, int toSub);
		Vector4 operator-(Vector4 const& src, double toSub);
		Vector4 operator-(Vector4 const& src, Vector4 const& toSub);
		Vector4 operator*(Vector4 const& src, float toMult);
		Vector4 operator*(Vector4 const& src, int toMult);
		Vector4 operator*(Vector4 const& src, double toMult);
		Vector4 operator*(Vector4 const& src, Vector4 const& toMult);
		Vector4 operator/(Vector4 const& src, float toDiv);
		Vector4 operator/(Vector4 const& src, int toDiv);
		Vector4 operator/(Vector4 const& src, double toDiv);
		Vector4 operator/(Vector4 const& src, Vector4 const& toDiv);
	}
}