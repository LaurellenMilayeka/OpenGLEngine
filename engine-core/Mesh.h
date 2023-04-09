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
			std::vector<std::vector<Engine::Maths::Vector3>> TextureCoords;
			std::vector<Engine::Maths::Vector3> Normals;
			std::vector<unsigned int> Indices;
			unsigned int              Offset;
			bool                      IsEnabled;
			bool                      Is3DTexture;
			GLuint                    EBO;

			Mesh()
				: Mat(nullptr),
				Is3DTexture(false),
				IsEnabled(true),
				Offset(0)
			{
				EBO = 0;
			}

			void Compute()
			{
				GLuint VBO = 0;
				unsigned int vertexArrayIndex = 0;

				size_t bufferPointer = 0;
				size_t bufferSize = (sizeof(Engine::Maths::Vector3) * Vertices.size());

				for (std::vector<Engine::Maths::Vector3> const& coords : TextureCoords)
				{
					bufferSize += (coords.size() > 0) ?
						(sizeof(Engine::Maths::Vector3) * coords.size())
						: 0;
				}

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
				glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
				glEnableVertexAttribArray(vertexArrayIndex++);

				bufferPointer += sizeof(Engine::Maths::Vector3) * Vertices.size();

				if (Normals.size() > 0)
				{
					glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector3) * Normals.size(), &Normals[0]);
					glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
					glEnableVertexAttribArray(vertexArrayIndex++);

					bufferPointer += sizeof(Engine::Maths::Vector3) * Normals.size();
				}

				if (TextureCoords.size() > 0)
				{
					for (std::vector<Engine::Maths::Vector3> const& coords : TextureCoords)
					{
						if ( ! Is3DTexture )
						{
							glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector3) * coords.size(), &coords[0]);
							glVertexAttribPointer(vertexArrayIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
							glEnableVertexAttribArray(vertexArrayIndex++);

							bufferPointer += sizeof(Engine::Maths::Vector3) * coords.size();
						}
						else
						{
							glBufferSubData(GL_ARRAY_BUFFER, bufferPointer, sizeof(Engine::Maths::Vector3) * coords.size(), &coords[0]);
							glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Maths::Vector3), (void*)(bufferPointer));
							glEnableVertexAttribArray(vertexArrayIndex++);

							bufferPointer += sizeof(Engine::Maths::Vector3) * coords.size();
						}
					}
				}



				if (Indices.size() > 0)
				{
					glGenBuffers(1, &EBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
				glBindVertexArray(0);
			}
		};
	}
}


