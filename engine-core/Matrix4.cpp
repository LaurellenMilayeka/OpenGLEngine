#include "Matrix4.h"
#include <string>

using namespace Engine::Maths;

float Engine::Maths::Matrix4::_identity[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Matrix4 Engine::Maths::operator+(Matrix4 const& src, Matrix4 const& toAdd)
{
	float data[16] = {
		src[0] + toAdd[0],  src[1] + toAdd[1],  src[2] + toAdd[2],  src[3] + toAdd[3],
		src[4] + toAdd[4],  src[5] + toAdd[5],  src[6] + toAdd[6],  src[7] + toAdd[7],
		src[8] + toAdd[8],  src[9] + toAdd[9],  src[10] + toAdd[10], src[11] + toAdd[11],
		src[12] + toAdd[12], src[13] + toAdd[13], src[14] + toAdd[14], src[15] + toAdd[15]
	};

	return Matrix4(data);
}

Matrix4 Engine::Maths::operator-(Matrix4 const& src, Matrix4 const& toSub)
{
	float data[16] = {
		src[0] - toSub[0],  src[1] - toSub[1],  src[2] - toSub[2],  src[3] - toSub[3],
		src[4] - toSub[4],  src[5] - toSub[5],  src[6] - toSub[6],  src[7] - toSub[7],
		src[8] - toSub[8],  src[9] - toSub[9],  src[10] - toSub[10], src[11] - toSub[11],
		src[12] - toSub[12], src[13] - toSub[13], src[14] - toSub[14], src[15] - toSub[15]
	};

	return Matrix4(data);
}

Matrix4 Engine::Maths::operator*(Matrix4 const src, int toMult)
{
	float data[16] = {
		src[0] * toMult, src[1] * toMult, src[2] * toMult, src[3] * toMult,
		src[4] * toMult, src[5] * toMult, src[6] * toMult, src[7] * toMult,
		src[8] * toMult, src[9] * toMult, src[10] * toMult, src[11] * toMult,
		src[12] * toMult, src[13] * toMult, src[14] * toMult, src[15] * toMult,
	};
	return Matrix4(data);
}

Matrix4 Engine::Maths::operator*(Matrix4 const src, float toMult)
{
	float data[16] = {
		src[0] * toMult, src[1] * toMult, src[2] * toMult, src[3] * toMult,
		src[4] * toMult, src[5] * toMult, src[6] * toMult, src[7] * toMult,
		src[8] * toMult, src[9] * toMult, src[10] * toMult, src[11] * toMult,
		src[12] * toMult, src[13] * toMult, src[14] * toMult, src[15] * toMult
	};
	
	for (unsigned int i = 0; i < 16; i++)
	{
		if ((i % 4) == 0)
			std::cout << std::endl;
		std::cout << std::to_string(data[i]) << " ";
	}

	return Matrix4(data);
}

Matrix4 Engine::Maths::operator*(Matrix4 const src, double toMult)
{
	float data[16] = {
		src[0] * static_cast<float>(toMult), src[1] * static_cast<float>(toMult), src[2] * static_cast<float>(toMult), src[3] * static_cast<float>(toMult),
		src[4] * static_cast<float>(toMult), src[5] * static_cast<float>(toMult), src[6] * static_cast<float>(toMult), src[7] * static_cast<float>(toMult),
		src[8] * static_cast<float>(toMult), src[9] * static_cast<float>(toMult), src[10] * static_cast<float>(toMult), src[11] * static_cast<float>(toMult),
		src[12] * static_cast<float>(toMult), src[13] * static_cast<float>(toMult), src[14] * static_cast<float>(toMult), src[15] * static_cast<float>(toMult),
	};
	return Matrix4(data);
}

Matrix4 Engine::Maths::operator*(Matrix4 const& mat1, Matrix4 const& mat2)
{
	float data[16];

	data[0] = (mat1[0] * mat2[0]) + (mat1[1] * mat2[4]) + (mat1[2] * mat2[8]) + (mat1[3] * mat2[12]);
	data[1] = (mat1[0] * mat2[1]) + (mat1[1] * mat2[5]) + (mat1[2] * mat2[9]) + (mat1[3] * mat2[13]);
	data[2] = (mat1[0] * mat2[2]) + (mat1[1] * mat2[6]) + (mat1[2] * mat2[10]) + (mat1[3] * mat2[14]);
	data[3] = (mat1[0] * mat2[3]) + (mat1[1] * mat2[7]) + (mat1[2] * mat2[11]) + (mat1[3] * mat2[15]);

	data[4] = (mat1[4] * mat2[0]) + (mat1[5] * mat2[4]) + (mat1[6] * mat2[8]) + (mat1[7] * mat2[12]);
	data[5] = (mat1[4] * mat2[1]) + (mat1[5] * mat2[5]) + (mat1[6] * mat2[9]) + (mat1[7] * mat2[13]);
	data[6] = (mat1[4] * mat2[2]) + (mat1[5] * mat2[6]) + (mat1[6] * mat2[10]) + (mat1[7] * mat2[14]);
	data[7] = (mat1[4] * mat2[3]) + (mat1[5] * mat2[7]) + (mat1[6] * mat2[11]) + (mat1[7] * mat2[15]);

	data[8] = (mat1[8] * mat2[0]) + (mat1[9] * mat2[4]) + (mat1[10] * mat2[8]) + (mat1[11] * mat2[12]);
	data[9] = (mat1[8] * mat2[1]) + (mat1[9] * mat2[5]) + (mat1[10] * mat2[9]) + (mat1[11] * mat2[13]);
	data[10] = (mat1[8] * mat2[2]) + (mat1[9] * mat2[6]) + (mat1[10] * mat2[10]) + (mat1[11] * mat2[14]);
	data[11] = (mat1[8] * mat2[3]) + (mat1[9] * mat2[7]) + (mat1[10] * mat2[11]) + (mat1[11] * mat2[15]);

	data[12] = (mat1[12] * mat2[0]) + (mat1[13] * mat2[4]) + (mat1[14] * mat2[8]) + (mat1[15] * mat2[12]);
	data[13] = (mat1[12] * mat2[1]) + (mat1[13] * mat2[5]) + (mat1[14] * mat2[9]) + (mat1[15] * mat2[13]);
	data[14] = (mat1[12] * mat2[2]) + (mat1[13] * mat2[6]) + (mat1[14] * mat2[10]) + (mat1[15] * mat2[14]);
	data[15] = (mat1[12] * mat2[3]) + (mat1[13] * mat2[7]) + (mat1[14] * mat2[11]) + (mat1[15] * mat2[15]);

	return Matrix4(data);
}

Vector4 Engine::Maths::operator*(Matrix4 const& mat1, Vector4 const& vec1)
{
	Vector4 toReturn = {};

	toReturn.x = (mat1[0] * vec1.x) + (mat1[1] * vec1.y) + (mat1[2] * vec1.z) + (mat1[3] * vec1.w);
	toReturn.y = (mat1[4] * vec1.x) + (mat1[5] * vec1.y) + (mat1[6] * vec1.z) + (mat1[7] * vec1.w);
	toReturn.z = (mat1[8] * vec1.x) + (mat1[9] * vec1.y) + (mat1[10] * vec1.z) + (mat1[11] * vec1.w);
	toReturn.w = (mat1[12] * vec1.x) + (mat1[13] * vec1.y) + (mat1[14] * vec1.z) + (mat1[15] * vec1.w);
	return toReturn;
}

std::ostream& Engine::Maths::operator<<(std::ostream& out, Matrix4 const& toDisplay)
{
	out << "| " << toDisplay[0] << " " << toDisplay[1] << " " << toDisplay[2] << " " << toDisplay[3] << " |" << std::endl;
	out << "| " << toDisplay[4] << " " << toDisplay[5] << " " << toDisplay[6] << " " << toDisplay[7] << " |" << std::endl;
	out << "| " << toDisplay[8] << " " << toDisplay[9] << " " << toDisplay[10] << " " << toDisplay[11] << " |" << std::endl;
	out << "| " << toDisplay[12] << " " << toDisplay[13] << " " << toDisplay[14] << " " << toDisplay[15] << " |";
	return out;
}
