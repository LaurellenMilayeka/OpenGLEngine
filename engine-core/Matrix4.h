#pragma once

#include <iostream>
#include <array>
#include <cassert>

#include "Vector4.h"

namespace Engine
{
	namespace Maths
	{
		struct Matrix4
		{
		private:

			float _data[16] = {};
			static float _identity[16];

		public:
			
			Matrix4()
			{
				for (unsigned int index = 0; index < 16; index++)
				{
					_data[index] = _identity[index];
				}
			}

			Matrix4(float* data)
			{
				for (unsigned int index = 0; index < 16; index++)
				{
					_data[index] = data[index];
				}
			}

			static Matrix4 Identity()
			{
				return _identity;
			}

			float const* Data() const
			{
				return _data;
			}

			float Determinant() const
			{
				return (
					(At(0, 0) * At(1, 1) * At(2, 2) * At(3, 3)) + (At(0, 0) * At(1, 2) * At(2, 3) * At(3, 1)) + (At(0, 0) * At(1, 3) * At(2, 1) * At(3, 2))
				-	(At(0, 0) * At(1, 3) * At(2, 2) * At(3, 1)) - (At(0, 0) * At(1, 2) * At(2, 1) * At(3, 3)) - (At(0, 0) * At(1, 1) * At(2, 3) * At(3, 2))
				-	(At(0, 1) * At(1, 0) * At(2, 2) * At(3, 3)) - (At(0, 2) * At(1, 0) * At(2, 3) * At(3, 1)) - (At(0, 3) * At(1, 0) * At(2, 1) * At(3, 2))
				+	(At(0, 3) * At(1, 0) * At(2, 2) * At(3, 1)) + (At(0, 2) * At(1, 0) * At(2, 1) * At(3, 3)) + (At(0, 1) * At(1, 0) * At(2, 3) * At(3, 2))
				+	(At(0, 1) * At(1, 2) * At(2, 0) * At(3, 3)) + (At(0, 2) * At(1, 3) * At(2, 0) * At(3, 1)) + (At(0, 3) * At(1, 1) * At(2, 0) * At(3, 2))
				-	(At(0, 3) * At(1, 2) * At(2, 0) * At(3, 1)) - (At(0, 2) * At(1, 1) * At(2, 0) * At(3, 3)) - (At(0, 1) * At(1, 3) * At(2, 0) * At(3, 2))
				-	(At(0, 1) * At(1, 2) * At(2, 3) * At(3, 0)) - (At(0, 2) * At(1, 3) * At(2, 1) * At(3, 0)) - (At(0, 3) * At(1, 1) * At(2, 2) * At(3, 0))
				+	(At(0, 3) * At(1, 2) * At(2, 1) * At(3, 0)) + (At(0, 2) * At(1, 1) * At(2, 3) * At(3, 0)) + (At(0, 1) * At(1, 3) * At(2, 2) * At(3, 0))
					);
			}

			void Transpose()
			{
				float data[16] = {
					At(0, 0), At(1, 0), At(2, 0), At(3, 0),
					At(0, 1), At(1, 1), At(2, 1), At(3, 1),
					At(0, 2), At(1, 2), At(2, 2), At(3, 2),
					At(0, 3), At(1, 3), At(2, 3), At(3, 3),
				};
				
				for (unsigned int i = 0; i < 16; i++)
				{
					_data[i] = data[i];
				}
			}

			static Matrix4 Transpose(Matrix4 const& toTranspose)
			{
				float data[16] = {
					toTranspose.At(0, 0), toTranspose.At(0, 1), toTranspose.At(0, 2), toTranspose.At(0, 3),
					toTranspose.At(1, 0), toTranspose.At(1, 1), toTranspose.At(1, 2), toTranspose.At(1, 3),
					toTranspose.At(2, 0), toTranspose.At(2, 1), toTranspose.At(2, 2), toTranspose.At(2, 3),
					toTranspose.At(3, 0), toTranspose.At(3, 1), toTranspose.At(3, 2), toTranspose.At(3, 3),
				};
				return Matrix4(data);
			}

			static Matrix4 Invert(Matrix4 const& toInvert)
			{
				float detTmp[16] =
				{
					// M1-1
					(toInvert.At(1, 1) * toInvert.At(2, 2) * toInvert.At(3, 3)) + (toInvert.At(1, 2) * toInvert.At(2, 3) * toInvert.At(3, 1)) + (toInvert.At(1, 3) * toInvert.At(2, 1) * toInvert.At(3, 2))  //OK
				-	(toInvert.At(1, 3) * toInvert.At(2, 2) * toInvert.At(3, 1)) - (toInvert.At(1, 2) * toInvert.At(2, 1) * toInvert.At(3, 3)) - (toInvert.At(1, 1) * toInvert.At(2, 3) * toInvert.At(3, 2)), //OK
					
					// M1-2
					(toInvert.At(1, 0) * toInvert.At(2, 2) * toInvert.At(3, 3)) + (toInvert.At(1, 2) * toInvert.At(2, 3) * toInvert.At(3, 0)) + (toInvert.At(1, 3) * toInvert.At(2, 0) * toInvert.At(3, 2))  //OK
				-	(toInvert.At(1, 3) * toInvert.At(2, 2) * toInvert.At(3, 0)) - (toInvert.At(1, 2) * toInvert.At(2, 0) * toInvert.At(3, 3)) - (toInvert.At(1, 0) * toInvert.At(2, 3) * toInvert.At(3, 2)), //OK
					
					// M1-3
					(toInvert.At(1, 0) * toInvert.At(2, 1) * toInvert.At(3, 3)) + (toInvert.At(1, 1) * toInvert.At(2, 3) * toInvert.At(3, 0)) + (toInvert.At(1, 3) * toInvert.At(2, 0) * toInvert.At(3, 1))  //OK
				-	(toInvert.At(1, 3) * toInvert.At(2, 1) * toInvert.At(3, 0)) - (toInvert.At(1, 1) * toInvert.At(2, 0) * toInvert.At(3, 3)) - (toInvert.At(1, 0) * toInvert.At(2, 3) * toInvert.At(3, 1)), //OK
					
					// M1-4
					(toInvert.At(1, 0) * toInvert.At(2, 1) * toInvert.At(3, 2)) + (toInvert.At(1, 1) * toInvert.At(2, 2) * toInvert.At(3, 0)) + (toInvert.At(1, 2) * toInvert.At(2, 0) * toInvert.At(3, 1))  //OK
				-	(toInvert.At(1, 2) * toInvert.At(2, 1) * toInvert.At(3, 0)) - (toInvert.At(1, 1) * toInvert.At(2, 0) * toInvert.At(3, 2)) - (toInvert.At(1, 0) * toInvert.At(2, 2) * toInvert.At(3, 1)), //OK

					// M2-1
					(toInvert.At(0, 1) * toInvert.At(2, 2) * toInvert.At(3, 3)) + (toInvert.At(0, 2) * toInvert.At(2, 3) * toInvert.At(3, 1)) + (toInvert.At(0, 3) * toInvert.At(2, 1) * toInvert.At(3, 2))  //OK
				-	(toInvert.At(0, 3) * toInvert.At(2, 2) * toInvert.At(3, 1)) - (toInvert.At(0, 2) * toInvert.At(2, 1) * toInvert.At(3, 3)) - (toInvert.At(0, 1) * toInvert.At(2, 3) * toInvert.At(3, 2)), //OK
					
					// M2-2
					(toInvert.At(0, 0) * toInvert.At(2, 2) * toInvert.At(3, 3)) + (toInvert.At(0, 2) * toInvert.At(2, 3) * toInvert.At(3, 0)) + (toInvert.At(0, 3) * toInvert.At(2, 0) * toInvert.At(3, 2))  //OK
				-	(toInvert.At(0, 3) * toInvert.At(2, 2) * toInvert.At(3, 0)) - (toInvert.At(0, 2) * toInvert.At(2, 0) * toInvert.At(3, 3)) - (toInvert.At(0, 0) * toInvert.At(2, 3) * toInvert.At(3, 2)), //OK
					
					// M2-3
					(toInvert.At(0, 0) * toInvert.At(2, 1) * toInvert.At(3, 3)) + (toInvert.At(0, 1) * toInvert.At(2, 3) * toInvert.At(3, 0)) + (toInvert.At(0, 3) * toInvert.At(2, 0) * toInvert.At(3, 1))  //OK
				-	(toInvert.At(0, 3) * toInvert.At(2, 1) * toInvert.At(3, 0)) - (toInvert.At(0, 1) * toInvert.At(2, 0) * toInvert.At(3, 3)) - (toInvert.At(0, 0) * toInvert.At(2, 3) * toInvert.At(3, 1)), //OK
					
					// M2-4
					(toInvert.At(0, 0) * toInvert.At(2, 1) * toInvert.At(3, 2)) + (toInvert.At(0, 1) * toInvert.At(2, 2) * toInvert.At(3, 0)) + (toInvert.At(0, 2) * toInvert.At(2, 0) * toInvert.At(3, 1))
				-	(toInvert.At(0, 2) * toInvert.At(2, 1) * toInvert.At(3, 0)) - (toInvert.At(0, 1) * toInvert.At(2, 0) * toInvert.At(3, 2)) - (toInvert.At(0, 0) * toInvert.At(2, 2) * toInvert.At(3, 1)),

					// M3-1
					(toInvert.At(0, 1) * toInvert.At(1, 2) * toInvert.At(3, 3)) + (toInvert.At(0, 2) * toInvert.At(1, 3) * toInvert.At(3, 1)) + (toInvert.At(0, 3) * toInvert.At(1, 1) * toInvert.At(3, 2))
				-	(toInvert.At(0, 3) * toInvert.At(1, 2) * toInvert.At(3, 1)) - (toInvert.At(0, 2) * toInvert.At(1, 1) * toInvert.At(3, 3)) - (toInvert.At(0, 1) * toInvert.At(1, 3) * toInvert.At(3, 2)),
					
					// M3-2
					(toInvert.At(0, 0) * toInvert.At(1, 2) * toInvert.At(3, 3)) + (toInvert.At(0, 2) * toInvert.At(1, 3) * toInvert.At(3, 0)) + (toInvert.At(0, 3) * toInvert.At(1, 0) * toInvert.At(3, 2))
				-	(toInvert.At(0, 3) * toInvert.At(1, 2) * toInvert.At(3, 0)) - (toInvert.At(0, 2) * toInvert.At(1, 0) * toInvert.At(3, 3)) - (toInvert.At(0, 0) * toInvert.At(1, 3) * toInvert.At(3, 2)),
					
					// M3-3
					(toInvert.At(0, 0) * toInvert.At(1, 1) * toInvert.At(3, 3)) + (toInvert.At(0, 1) * toInvert.At(1, 3) * toInvert.At(3, 0)) + (toInvert.At(0, 3) * toInvert.At(1, 0) * toInvert.At(3, 1))
				-	(toInvert.At(0, 3) * toInvert.At(1, 1) * toInvert.At(3, 0)) - (toInvert.At(0, 1) * toInvert.At(1, 0) * toInvert.At(3, 3)) - (toInvert.At(0, 0) * toInvert.At(1, 3) * toInvert.At(3, 1)),
					
					// M3-4
					(toInvert.At(0, 0) * toInvert.At(1, 1) * toInvert.At(3, 2)) + (toInvert.At(0, 1) * toInvert.At(1, 2) * toInvert.At(3, 0)) + (toInvert.At(0, 2) * toInvert.At(1, 0) * toInvert.At(3, 1))
				-	(toInvert.At(0, 2) * toInvert.At(1, 1) * toInvert.At(3, 0)) - (toInvert.At(0, 1) * toInvert.At(1, 0) * toInvert.At(3, 2)) - (toInvert.At(0, 0) * toInvert.At(1, 2) * toInvert.At(3, 1)),

					// M4-1
					(toInvert.At(0, 1) * toInvert.At(1, 2) * toInvert.At(2, 3)) + (toInvert.At(0, 2) * toInvert.At(1, 3) * toInvert.At(2, 1)) + (toInvert.At(0, 3) * toInvert.At(1, 1) * toInvert.At(2, 2))
				-	(toInvert.At(0, 3) * toInvert.At(1, 2) * toInvert.At(2, 1)) - (toInvert.At(0, 2) * toInvert.At(1, 1) * toInvert.At(2, 3)) - (toInvert.At(0, 1) * toInvert.At(1, 3) * toInvert.At(2, 2)),
					
					// M4-2
					(toInvert.At(0, 0) * toInvert.At(1, 2) * toInvert.At(2, 3)) + (toInvert.At(0, 2) * toInvert.At(1, 3) * toInvert.At(2, 0)) + (toInvert.At(0, 3) * toInvert.At(1, 0) * toInvert.At(2, 2))
				-	(toInvert.At(0, 3) * toInvert.At(1, 2) * toInvert.At(2, 0)) - (toInvert.At(0, 2) * toInvert.At(1, 0) * toInvert.At(2, 3)) - (toInvert.At(0, 0) * toInvert.At(1, 3) * toInvert.At(2, 2)),
					
					// M4-3
					(toInvert.At(0, 0) * toInvert.At(1, 1) * toInvert.At(2, 3)) + (toInvert.At(0, 1) * toInvert.At(1, 3) * toInvert.At(2, 0)) + (toInvert.At(0, 3) * toInvert.At(1, 0) * toInvert.At(2, 1))
				-	(toInvert.At(0, 3) * toInvert.At(1, 1) * toInvert.At(2, 0)) - (toInvert.At(0, 1) * toInvert.At(1, 0) * toInvert.At(2, 3)) - (toInvert.At(0, 0) * toInvert.At(1, 3) * toInvert.At(2, 1)),
					
					// M4-4
					(toInvert.At(0, 0) * toInvert.At(1, 1) * toInvert.At(2, 2)) + (toInvert.At(0, 1) * toInvert.At(1, 2) * toInvert.At(2, 0)) + (toInvert.At(0, 2) * toInvert.At(1, 0) * toInvert.At(2, 1))
				-	(toInvert.At(0, 2) * toInvert.At(1, 1) * toInvert.At(2, 0)) - (toInvert.At(0, 1) * toInvert.At(1, 0) * toInvert.At(2, 2)) - (toInvert.At(0, 0) * toInvert.At(1, 2) * toInvert.At(2, 1))
				};
				Matrix4 tmp(detTmp);

				float data[16] = {
					(static_cast<float>(pow(-1, (1 + 1))) * tmp.At(0, 0)), (static_cast<float>(pow(-1, (1 + 2))) * tmp.At(1, 0)), (static_cast<float>(pow(-1, (1 + 3))) * tmp.At(2, 0)), (static_cast<float>(pow(-1, (1 + 4))) * tmp.At(3, 0)),
					(static_cast<float>(pow(-1, (2 + 1))) * tmp.At(0, 1)), (static_cast<float>(pow(-1, (2 + 2))) * tmp.At(1, 1)), (static_cast<float>(pow(-1, (2 + 3))) * tmp.At(2, 1)), (static_cast<float>(pow(-1, (2 + 4))) * tmp.At(3, 1)),
					(static_cast<float>(pow(-1, (3 + 1))) * tmp.At(0, 2)), (static_cast<float>(pow(-1, (3 + 2))) * tmp.At(1, 2)), (static_cast<float>(pow(-1, (3 + 3))) * tmp.At(2, 2)), (static_cast<float>(pow(-1, (3 + 4))) * tmp.At(2, 3)),
					(static_cast<float>(pow(-1, (4 + 1))) * tmp.At(0, 3)), (static_cast<float>(pow(-1, (4 + 2))) * tmp.At(1, 3)), (static_cast<float>(pow(-1, (4 + 3))) * tmp.At(3, 2)), (static_cast<float>(pow(-1, (4 + 4))) * tmp.At(3, 3))
				};
				Matrix4 ret(data);

				ret *= (1 / toInvert.Determinant());
				return ret;
			}

			float At(int x, int y) const
			{
				assert(4 > x);
				assert(4 > y);

				return _data[(y * 4) + x];
			}

			void SetAt(int x, int y, float value)
			{
				assert(4 > x);
				assert(4 > y);

				_data[(y * 4) + x] = value;
			}

			float operator[](int index) const
			{
				assert(16 > index);
				return _data[index];
			}

			void operator+=(Matrix4 const& toAdd)
			{												 
				_data[0]  = _data[0]  + toAdd[0];  _data[1]  = _data[1]  + toAdd[1];  _data[2]  = _data[2]  + toAdd[2];  _data[3]  = _data[3]  + toAdd[3];
				_data[4]  = _data[4]  + toAdd[4];  _data[5]  = _data[5]  + toAdd[5];  _data[6]  = _data[6]  + toAdd[6];  _data[7]  = _data[7]  + toAdd[7];
				_data[8]  = _data[8]  + toAdd[8];  _data[9]  = _data[9]  + toAdd[9];  _data[10] = _data[10] + toAdd[10]; _data[11] = _data[11] + toAdd[11];
				_data[12] = _data[12] + toAdd[12]; _data[13] = _data[13] + toAdd[13]; _data[14] = _data[14] + toAdd[14]; _data[15] = _data[15] + toAdd[15];
			}

			void operator-=(Matrix4 const& toSub)
			{
				_data[0]  = _data[0]  - toSub[0];  _data[1]  =  _data[1] - toSub[1];  _data[2]  = _data[2]  - toSub[2];  _data[3]  = _data[3]  - toSub[3];
				_data[4]  = _data[4]  - toSub[4];  _data[5]  =  _data[5] - toSub[5];  _data[6]  = _data[6]  - toSub[6];  _data[7]  = _data[7]  - toSub[7];
				_data[8]  = _data[8]  - toSub[8];  _data[9]  =  _data[9] - toSub[9];  _data[10] = _data[10] - toSub[10]; _data[11] = _data[11] - toSub[11];
				_data[12] = _data[12] - toSub[12]; _data[13] = _data[13] - toSub[13]; _data[14] = _data[14] - toSub[14]; _data[15] = _data[15] - toSub[15];
			}

			void operator*=(int toMult)
			{
				_data[0]  *= toMult; _data[1]  *= toMult; _data[2]  *= toMult; _data[3]  *= toMult;
				_data[4]  *= toMult; _data[5]  *= toMult; _data[6]  *= toMult; _data[7]  *= toMult;
				_data[8]  *= toMult; _data[9]  *= toMult; _data[10] *= toMult; _data[11] *= toMult;
				_data[12] *= toMult; _data[13] *= toMult; _data[14] *= toMult; _data[15] *= toMult;
			}

			void operator*=(double toMult)
			{
				_data[0]  *= static_cast<float>(toMult); _data[1]  *= static_cast<float>(toMult); _data[2]  *= static_cast<float>(toMult); _data[3]  *= static_cast<float>(toMult);
				_data[4]  *= static_cast<float>(toMult); _data[5]  *= static_cast<float>(toMult); _data[6]  *= static_cast<float>(toMult); _data[7]  *= static_cast<float>(toMult);
				_data[8]  *= static_cast<float>(toMult); _data[9]  *= static_cast<float>(toMult); _data[10] *= static_cast<float>(toMult); _data[11] *= static_cast<float>(toMult);
				_data[12] *= static_cast<float>(toMult); _data[13] *= static_cast<float>(toMult); _data[14] *= static_cast<float>(toMult); _data[15] *= static_cast<float>(toMult);
			}

			void operator*=(float toMult)
			{
				_data[0]  *= toMult; _data[1]  *= toMult; _data[2]  *= toMult; _data[3]  *= toMult;
				_data[4]  *= toMult; _data[5]  *= toMult; _data[6]  *= toMult; _data[7]  *= toMult;
				_data[8]  *= toMult; _data[9]  *= toMult; _data[10] *= toMult; _data[11] *= toMult;
				_data[12] *= toMult; _data[13] *= toMult; _data[14] *= toMult; _data[15] *= toMult;
			}

			void operator*=(Matrix4 const& toMult)
			{
				_data[0] = (_data[0] * toMult[0]) + (_data[1] * toMult[4]) + (_data[2] * toMult[8]) + (_data[3] * toMult[12]);
				_data[1] = (_data[0] * toMult[1]) + (_data[1] * toMult[5]) + (_data[2] * toMult[9]) + (_data[3] * toMult[13]);
				_data[2] = (_data[0] * toMult[2]) + (_data[1] * toMult[6]) + (_data[2] * toMult[10]) + (_data[3] * toMult[14]);
				_data[3] = (_data[0] * toMult[3]) + (_data[1] * toMult[7]) + (_data[2] * toMult[11]) + (_data[3] * toMult[15]);

				_data[4] = (_data[4] * toMult[0]) + (_data[5] * toMult[4]) + (_data[6] * toMult[8]) + (_data[7] * toMult[12]);
				_data[5] = (_data[4] * toMult[1]) + (_data[5] * toMult[5]) + (_data[6] * toMult[9]) + (_data[7] * toMult[13]);
				_data[6] = (_data[4] * toMult[2]) + (_data[5] * toMult[6]) + (_data[6] * toMult[10]) + (_data[7] * toMult[14]);
				_data[7] = (_data[4] * toMult[3]) + (_data[5] * toMult[7]) + (_data[6] * toMult[11]) + (_data[7] * toMult[15]);

				_data[8] = (_data[8] * toMult[0]) + (_data[9] * toMult[4]) + (_data[10] * toMult[8]) + (_data[11] * toMult[12]);
				_data[9] = (_data[8] * toMult[1]) + (_data[9] * toMult[5]) + (_data[10] * toMult[9]) + (_data[11] * toMult[13]);
				_data[10] = (_data[8] * toMult[2]) + (_data[9] * toMult[6]) + (_data[10] * toMult[10]) + (_data[11] * toMult[14]);
				_data[11] = (_data[8] * toMult[3]) + (_data[9] * toMult[7]) + (_data[10] * toMult[11]) + (_data[11] * toMult[15]);

				_data[12] = (_data[12] * toMult[0]) + (_data[13] * toMult[4]) + (_data[14] * toMult[8]) + (_data[15] * toMult[12]);
				_data[13] = (_data[12] * toMult[1]) + (_data[13] * toMult[5]) + (_data[14] * toMult[9]) + (_data[15] * toMult[13]);
				_data[14] = (_data[12] * toMult[2]) + (_data[13] * toMult[6]) + (_data[14] * toMult[10]) + (_data[15] * toMult[14]);
				_data[15] = (_data[12] * toMult[3]) + (_data[13] * toMult[7]) + (_data[14] * toMult[11]) + (_data[15] * toMult[15]);
			}

		};

		Matrix4 operator+(Matrix4 const& src, Matrix4 const& toAdd);
		Matrix4 operator-(Matrix4 const& src, Matrix4 const& toSub);
		Matrix4 operator*(Matrix4 const src, int toMult);
		Matrix4 operator*(Matrix4 const src, float toMult);
		Matrix4 operator*(Matrix4 const src, double toMult);
		Matrix4 operator*(Matrix4 const& mat1, Matrix4 const& mat2);
		Vector4 operator*(Matrix4 const& mat1, Vector4 const& vec1);

		std::ostream& operator<<(std::ostream& out, Matrix4 const& toDisplay);
	}
}