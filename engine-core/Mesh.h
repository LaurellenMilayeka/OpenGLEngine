#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"

namespace Engine
{
	namespace Graphics
	{
		struct Mesh
		{
			GLuint ID = 0;

			std::string GroupName;
			Material* Mat;

			std::vector<Engine::Maths::Vector3> Vertices;
			std::vector<Engine::Maths::Vector2> TextureCoords;
			std::vector<Engine::Maths::Vector3> Normals;
			std::vector<Engine::Maths::Vector3> Indices;

			Mesh()
				: Mat(nullptr)
			{

			}

			void Compute()
			{
				GLuint VBO = 0;
				GLuint EBO = 0;

				size_t bufferPointer = 0;
				size_t bufferSize = (sizeof(Engine::Maths::Vector3) * Vertices.size());
				bufferSize += (TextureCoords.size() > 0) ?
								(sizeof(Engine::Maths::Vector2) * TextureCoords.size())
							  : 0;
				bufferSize += (Normals.size() > 0) ?
								(sizeof(Engine::Maths::Vector3) * Normals.size())
							  : 0;


				if (ID == 0)
				{
					glGenVertexArrays(1, &ID);
				}

				glBindVertexArray(ID);
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, bufferSize, 0, GL_STATIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector3) * Vertices.size(), &Vertices[0]);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
				glEnableVertexAttribArray(0);

				bufferPointer += sizeof(Engine::Maths::Vector3) * Vertices.size();

				if (TextureCoords.size() > 0)
				{
					glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector2) * TextureCoords.size(), &TextureCoords[0]);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector2), (void*)(bufferPointer));
					glEnableVertexAttribArray(1);

					bufferPointer += sizeof(Engine::Maths::Vector2) * TextureCoords.size();
				}

				if (Normals.size() > 0)
				{
					glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector3) * Normals.size(), &Normals[0]);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
					glEnableVertexAttribArray(2);

					bufferPointer += sizeof(Engine::Maths::Vector3) * Normals.size();
				}
				glBindVertexArray(0);
			}
		};
	}
}


