#include "Transform.h"

#include <iostream>
#include <imgui.h>

using namespace Engine::Components;

Transform::Transform(Engine::Entity::Entity* parent) : IComponent("Transform", ComponentsID::Transform, parent)
{

}

Transform::~Transform()
{

}

Engine::Maths::Vector3 Transform::Position() const
{
	return Engine::Maths::Vector3(_mTranslationMatrix[3], _mTranslationMatrix[7], _mTranslationMatrix[11]);
}

Engine::Maths::Vector3 Transform::Scale() const
{
	return Engine::Maths::Vector3(_mScaleMatrix[0], _mScaleMatrix[5], _mScaleMatrix[10]);
}

Engine::Maths::Vector3 Transform::Rotation() const
{
	return Engine::Maths::Vector3(0.0f, 0.0f, 0.0f);
}

/*
	Matrix Scaling :

	--------------		-----------------------------------
	| 1  0  0  0 |		| (Sx) * 1      0         0     0 |
	| 0  1  0  0 |  --> |    0      (Sy) * 1      0     0 |
	| 0  0  1  0 |		|    0          0     (Sz) * 1  0 |
	| 0  0  0  1 |		|    0          0         0     1 |
	--------------		-----------------------------------
*/

void Transform::setRotation(float deg, Engine::Maths::Vector3 const& newRotation)
{
	Engine::Maths::Vector3 normAxis = Engine::Maths::Vector3::Normalize(newRotation);
	Engine::Maths::Vector3 tmp = (normAxis * (1 - cos(deg)));

	float data[16] = {
		cos(deg) + tmp.x * normAxis.x,
		tmp.x * normAxis.y - (normAxis.z * sin(deg)),
		tmp.x * normAxis.z + (normAxis.y * sin(deg)),
		0,

		tmp.y * normAxis.x + normAxis.z * sin(deg),
		cos(deg) + tmp.y * normAxis.y,
		tmp.y * normAxis.z - normAxis.x * sin(deg),
		0,

		tmp.z * normAxis.x - normAxis.y * sin(deg),
		tmp.z * normAxis.y + normAxis.x * sin(deg),
		cos(deg) + tmp.z * normAxis.z,
		0,

		0, 0, 0, 1
	};

	_mRotationMatrix = data;
}

Engine::Maths::Matrix4 Transform::GetModelMatrix() const
{
	return Engine::Maths::Matrix4::Transpose(_mTranslationMatrix * _mRotationMatrix * _mScaleMatrix);
}

void Transform::Translate(Engine::Maths::Vector3 const& newPoint)
{
	float data[16] = {
		1, 0, 0, newPoint.x,
		0, 1, 0, newPoint.y,
		0, 0, 1, newPoint.z,
		0, 0, 0,		  1
	};

	_mTranslationMatrix *= data;
}

void Transform::Rotate(float deg, Engine::Maths::Vector3 const& newRotation)
{
	setRotation(deg, newRotation);
}

void Transform::Scale(Engine::Maths::Vector3 const& newScale)
{
	_mScaleMatrix.SetAt(0, 0, newScale.x);
	_mScaleMatrix.SetAt(1, 1, newScale.y);
	_mScaleMatrix.SetAt(2, 2, newScale.z);
}
