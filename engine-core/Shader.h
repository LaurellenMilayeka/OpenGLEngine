#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "File.h"

namespace Engine
{
	namespace Misc
	{
		struct Shader
		{
			GLuint ID = 0;

			bool HasFragment = false;
			bool HasVertex = false;
			
			std::string FragmentShaderFilename;
			std::string	VertexShaderFilename;

			Shader() = default;

			Shader(std::string const& vertShaderFile, std::string const& fragShaderFile)
			{
				char logErr[255] = {};
				GLuint fragShader, vertShader;
				GLint successVert, successFrag, successProg;
				std::string fragShaderContent, vertShaderContent;

				Engine::Filesystem::File fragShaderFileContent(fragShaderFile);
				Engine::Filesystem::File vertShaderFileContent(vertShaderFile);

				if (fragShaderFileContent.IsValid())
				{
					fragShaderFileContent.Read(fragShaderContent, fragShaderFileContent.Length());
				}

				if (vertShaderFileContent.IsValid())
				{
					vertShaderFileContent.Read(vertShaderContent, vertShaderFileContent.Length());
				}

				char* fragContent = const_cast<char*>(fragShaderContent.c_str());
				char* vertContent = const_cast<char*>(vertShaderContent.c_str());

				fragShader = glCreateShader(GL_FRAGMENT_SHADER);
				vertShader = glCreateShader(GL_VERTEX_SHADER);

				glShaderSource(fragShader, 1, &fragContent, nullptr);
				glCompileShader(fragShader);
				glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successFrag);

				if (!successFrag)
				{
					glGetShaderInfoLog(fragShader, 255, nullptr, logErr);
					HasFragment = false;
					std::cout << logErr << std::endl;
					memset(logErr, 0, sizeof(logErr));
				}
				else
				{
					HasFragment = true;
				}

				glShaderSource(vertShader, 1, &vertContent, nullptr);
				glCompileShader(vertShader);
				glGetShaderiv(vertShader, GL_COMPILE_STATUS, &successVert);

				if (!successVert)
				{
					glGetShaderInfoLog(vertShader, 255, nullptr, logErr);
					HasVertex = false;
					std::cout << logErr << std::endl;
					memset(logErr, 0, sizeof(logErr));
				}
				else
				{
					HasVertex = true;
				}

				if (!HasFragment)
				{
					std::cout << "Fragment shader is missing." << std::endl;
				}

				if (!HasVertex)
				{
					std::cout << "Vertex shader is missing." << std::endl;
				}

				ID = glCreateProgram();
				glAttachShader(ID, fragShader);
				glAttachShader(ID, vertShader);
				glLinkProgram(ID);

				glGetProgramiv(ID, GL_LINK_STATUS, &successProg);
				if (!successProg)
				{
					glGetProgramInfoLog(ID, 255, nullptr, logErr);
					std::cout << logErr << std::endl;
					memset(logErr, 0, sizeof(logErr));
				}

				glDeleteShader(fragShader);
				glDeleteShader(vertShader);
			}

			void Use() const
			{
				glUseProgram(ID);
			}

			template<typename T>
			bool SetValue(std::string const& uniformName, T const& value) const
			{
				throw std::exception("Not Implemented");
			}

			template<>
			bool SetValue(std::string const& uniformName, glm::mat4 const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<Engine::Maths::Matrix4>(std::string const& uniformName, Engine::Maths::Matrix4 const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value.Data());
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<Engine::Maths::Vector4>(std::string const& uniformName, Engine::Maths::Vector4 const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform4f(uniformLoc, 
						static_cast<GLfloat>(value.x), static_cast<GLfloat>(value.y),
						static_cast<GLfloat>(value.z), static_cast<GLfloat>(value.w));
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<Engine::Maths::Vector3>(std::string const& uniformName, Engine::Maths::Vector3 const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform3f(uniformLoc,
						static_cast<GLfloat>(value.x),
						static_cast<GLfloat>(value.y),
						static_cast<GLfloat>(value.z));
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<Engine::Maths::Vector2>(std::string const& uniformName, Engine::Maths::Vector2 const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform2f(uniformLoc,
						static_cast<GLfloat>(value.x),
						static_cast<GLfloat>(value.y));
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<bool>(std::string const& uniformName, bool const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform1i(uniformLoc, value);
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<int>(std::string const& uniformName, int const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform1i(uniformLoc, value);
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<unsigned int>(std::string const& uniformName, unsigned int const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform1i(uniformLoc, value);
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<float>(std::string const& uniformName, float const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform1f(uniformLoc, value);
					return true;
				}
				return false;
			}

			template<>
			bool SetValue<double>(std::string const& uniformName, double const& value) const
			{
				GLuint uniformLoc = glGetUniformLocation(ID, uniformName.c_str());

				if (uniformLoc != -1)
				{
					glUniform1d(uniformLoc, value);
					return true;
				}
				return false;
			}

			void Destroy()
			{
				glDeleteProgram(ID);
			}
		};
	}
}