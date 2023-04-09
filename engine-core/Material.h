#pragma once

#include "Vector4.h"
#include "Texture.h"

namespace Engine
{
	namespace Graphics
	{
		struct Material
		{
			std::string Name;

			Engine::Maths::Vector4 Ambient;
			Engine::Graphics::Texture* AmbientTexture;
			Engine::Maths::Vector4 Diffuse;
			Engine::Graphics::Texture* DiffuseTexture;
			Engine::Maths::Vector4 Specular;
			Engine::Graphics::Texture* SpecularTexture;
			float				  SpecularExponent;
			float				  Opacity = 1.0f;
			float				  Illumination;
			float                 RefractionIndex;

			Material()
				: AmbientTexture(nullptr),
				DiffuseTexture(nullptr),
				SpecularTexture(nullptr),
				SpecularExponent(0.0f),
				Opacity(0.0f),
				Illumination(0.0f),
				RefractionIndex(0.0f)
			{

			}
		};
	}
}