#pragma once

#include <math.h>

namespace Engine
{
	namespace Maths
	{
		struct Vector2
		{
			float x;
			float y;

			Vector2()
			{
				x = 0;
				y = 0;
			}

			Vector2(float xy)
			{
				x = xy;
				y = xy;
			}
			
			Vector2(float x, float y)
			{
				this->x = x;
				this->y = y;
			}

			static Vector2 Zero()
			{
				return Vector2(0.0f, 0.0f);
			}

			static Vector2 Up()
			{
				return Vector2(0.0f, 1.0f);
			}

			static Vector2 Down()
			{
				return Vector2(0.0f, -1.0f);
			}

			static Vector2 Left()
			{
				return Vector2(-1.0f, 0.0f);
			}

			static Vector2 Right()
			{
				return Vector2(1.0f, 0.0f);
			}

			void Negate()
			{
				this->x *= -1;
				this->y *= -1;
			}

			float Magnitude()
			{
				return sqrtf((this->x * this->x) + (this->y * this->y));
			}

			float Length()
			{
				return Magnitude();
			}

			float Dot(Vector2 const& toDot)
			{
				return (this->x * toDot.x) + (this->y * toDot.y);
			}

			void operator-()
			{
				this->x *= -1;
				this->y *= -1;
			}

			void operator+=(float toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
			}

			void operator+=(int toAdd)
			{
				this->x += toAdd;
				this->y += toAdd;
			}

			void operator+=(double toAdd)
			{
				this->x += static_cast<float>(toAdd);
				this->y += static_cast<float>(toAdd);
			}

			void operator+=(Vector2 const& toAdd)
			{
				this->x += toAdd.x;
				this->y += toAdd.y;
			}

			void operator-=(float toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
			}

			void operator-=(int toSub)
			{
				this->x -= toSub;
				this->y -= toSub;
			}

			void operator-=(double toSub)
			{
				this->x -= static_cast<float>(toSub);
				this->y -= static_cast<float>(toSub);
			}

			void operator-=(Vector2 const& toSub)
			{
				this->x -= toSub.x;
				this->y -= toSub.y;
			}

			void operator*=(float toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
			}

			void operator*=(int toMult)
			{
				this->x *= toMult;
				this->y *= toMult;
			}

			void operator*=(double toMult)
			{
				this->x *= static_cast<float>(toMult);
				this->y *= static_cast<float>(toMult);
			}

			void operator*=(Vector2 const& toMult)
			{
				this->x *= toMult.x;
				this->y *= toMult.y;
			}

			void operator/=(float toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
			}

			void operator/=(int toDiv)
			{
				this->x /= toDiv;
				this->y /= toDiv;
			}

			void operator/=(double toDiv)
			{
				this->x /= static_cast<float>(toDiv);
				this->y /= static_cast<float>(toDiv);
			}

			void operator/=(Vector2 const& toDiv)
			{
				this->x /= toDiv.x;
				this->y /= toDiv.y;
			}

			bool operator!=(Vector2 const& toCmp)
			{
				return this->x != toCmp.x
					&& this->y != toCmp.y;
			}
		};

		Vector2 operator+(Vector2 const& src, float toAdd);
		Vector2 operator+(Vector2 const& src, int toAdd);
		Vector2 operator+(Vector2 const& src, double toAdd);
		Vector2 operator+(Vector2 const& src, Vector2 const& toAdd);
		Vector2 operator-(Vector2 const& src, float toSub);
		Vector2 operator-(Vector2 const& src, int toSub);
		Vector2 operator-(Vector2 const& src, double toSub);
		Vector2 operator-(Vector2 const& src, Vector2 const& toSub);
		Vector2 operator*(Vector2 const& src, float toMult);
		Vector2 operator*(Vector2 const& src, int toMult);
		Vector2 operator*(Vector2 const& src, double toMult);
		Vector2 operator*(Vector2 const& src, Vector2 const& toMult);
		Vector2 operator/(Vector2 const& src, float toDiv);
		Vector2 operator/(Vector2 const& src, int toDiv);
		Vector2 operator/(Vector2 const& src, double toDiv);
		Vector2 operator/(Vector2 const& src, Vector2 const& toDiv);
	}
}

