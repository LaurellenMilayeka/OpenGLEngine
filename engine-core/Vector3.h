#pragma once

#include <iostream>
#include <math.h>
#include "Vector2.h"

namespace Engine
{
	namespace Maths
	{
		struct Vector3
		{
			float x;
			float y;
			float z;

			Vector3()
			{
				x = 0;
				y = 0;
				z = 0;
			}

			Vector3(Vector2 const& vec)
			{
				x = vec.x;
				y = vec.y;
				z = 0.0f;
			}

			Vector3(float xyz)
			{
				x = xyz;
				y = xyz;
				z = xyz;
			}

			Vector3(float x, float y, float z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}

			static Vector3 Zero()
			{
				return Vector3(0.0f, 0.0f, 0.0f);
			}

			static Vector3 Up()
			{
				return Vector3(0.0f, 1.0f, 0.0f);
			}

			static Vector3 Down()
			{
				return Vector3(0.0f, -1.0f, 0.0f);
			}

			static Vector3 Left()
			{
				return Vector3(-1.0f, 0.0f, 0.0f);
			}

			static Vector3 Right()
			{
				return Vector3(1.0f, 0.0f, 0.0f);
			}

			static Vector3 Forward()
			{
				return Vector3(0.0f, 0.0f, 1.0f);
			}

			static Vector3 Backward()
			{
				return Vector3(0.0f, 0.0f, -1.0f);
			}

			void Negate()
			{
				this->x *= -1;
				this->y *= -1;
				this->z *= -1;
			}

			float Magnitude() const
			{
				return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
			}

			float Length() const
			{
				return Magnitude();
			}

			void Normalize()
			{
				this->x /= Length();
				this->y /= Length();
				this->z /= Length();
			}

			static Vector3 Normalize(Vector3 const& toNormalize)
			{
				return Vector3(toNormalize.x / toNormalize.Length(), toNormalize.y / toNormalize.Length(), toNormalize.z / toNormalize.Length());
			}

			float Dot(Vector3 const& toDot)

			{
				return (this->x * toDot.x) + (this->y * toDot.y) + (this->z * toDot.z);
			}
			
			static float Dot(Vector3 const& vec1, Vector3 const& vec2)
			{
				return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
			}

			Vector3 Cross(Vector3 const& toCross)
			{
				return Vector3
					((this->y * toCross.z) - (this->z * toCross.y),
					 (this->z * toCross.x) - (this->x * toCross.z),
					 (this->x * toCross.y) - (this->y * toCross.x));
			}

			static Vector3 Cross(Vector3 const& vec1, Vector3 const& vec2)
			{
				return Vector3
					((vec1.y * vec2.z) - (vec1.z * vec2.y),
					 (vec1.z * vec2.x) - (vec1.x * vec2.z),
					 (vec1.x * vec2.y) - (vec1.y * vec2.x));

			}

			Vector3& operator-()
			{
				this->x *= -1;
				this->y *= -1;
				this->z *= -1;
				return *this;
			}

			void operator+=(float toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
				this->z += toAdd;
			}

			void operator+=(int toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
				this->z += toAdd;
			}

			void operator+=(double toAdd)
			{
				this->x += static_cast<float>(toAdd);
				this->y += static_cast<float>(toAdd);
				this->z += static_cast<float>(toAdd);
			}

			void operator+=(Vector3 const& toAdd)
			{
				this->x += toAdd.x;
				this->y += toAdd.y;
				this->z += toAdd.z;
			}

			void operator-=(float toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
				this->z -= toSub;
			}

			void operator-=(int toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
				this->z -= toSub;
			}

			void operator-=(double toSub)
			{
				this->x -= static_cast<float>(toSub);
				this->y -= static_cast<float>(toSub);
				this->z -= static_cast<float>(toSub);
			}

			void operator-=(Vector3 const& toSub)
			{
				this->x -= toSub.x;
				this->y -= toSub.y;
				this->z -= toSub.z;
			}

			void operator*=(float toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
				this->z *= toMult;
			}

			void operator*=(int toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
				this->z *= toMult;
			}

			void operator*=(double toMult)
			{
				this->x *= static_cast<float>(toMult);
				this->y *= static_cast<float>(toMult);
				this->z *= static_cast<float>(toMult);
			}

			void operator*=(Vector3 const& toMult)
			{
				this->x *= toMult.x;
				this->y *= toMult.y;
				this->z *= toMult.z;
			}

			void operator/=(float toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
				this->z /= toDiv;
			}

			void operator/=(int toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
				this->z /= toDiv;
			}

			void operator/=(double toDiv)
			{
				this->x /= static_cast<float>(toDiv);
				this->y /= static_cast<float>(toDiv);
				this->z /= static_cast<float>(toDiv);
			}

			void operator/=(Vector3 const& toDiv)
			{
				this->x /= toDiv.x;
				this->y /= toDiv.y;
				this->z /= toDiv.z;
			}
		};

		Vector3 operator+(Vector3 const& src, float toAdd);
		Vector3 operator+(Vector3 const& src, int toAdd);
		Vector3 operator+(Vector3 const& src, double toAdd);
		Vector3 operator+(Vector3 const& src, Vector3 const& toAdd);
		Vector3 operator-(Vector3 const& src, float toSub);
		Vector3 operator-(Vector3 const& src, int toSub);
		Vector3 operator-(Vector3 const& src, double toSub);
		Vector3 operator-(Vector3 const& src, Vector3 const& toSub);
		Vector3 operator*(Vector3 const& src, float toMult);
		Vector3 operator*(Vector3 const& src, int toMult);
		Vector3 operator*(Vector3 const& src, double toMult);
		Vector3 operator*(Vector3 const& src, Vector3 const& toMult);
		Vector3 operator/(Vector3 const& src, float toDiv);
		Vector3 operator/(Vector3 const& src, int toDiv);
		Vector3 operator/(Vector3 const& src, double toDiv);
		Vector3 operator/(Vector3 const& src, Vector3 const& toDiv);

		std::ostream& operator<<(std::ostream& out, Vector3 const& toDisplay);
	}
}

