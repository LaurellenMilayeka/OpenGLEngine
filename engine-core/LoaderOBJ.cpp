#include "Loader.h"
#include "File.h"
#include "Helpers.h"

#include <vector>
#include <sstream>

using namespace Engine::InternalIO;
using namespace Engine::Filesystem;

std::string Loader<ModelType::OBJ>::_mBasePath;

static void ExtractFloat(std::string const& line, float& value)
{
	std::stringstream substrLine;

	substrLine << line;
	substrLine >> value;
}

static void ExtractVector3(std::string const& line, Engine::Maths::Vector3& vec)
{
	std::stringstream substrLine;

	substrLine << line;
	substrLine >> vec.x >> vec.y >> vec.z;
}

static void ExtractVector2(std::string const& line, Engine::Maths::Vector2& vec)
{
	std::stringstream substrLine;

	substrLine << line;
	substrLine >> vec.x >> vec.y;
}

static void ExtractFaces(std::string const& line, std::vector<unsigned int>& facesIndex)
{
	std::vector<std::string> faces;
	std::stringstream substrLine;
	std::string output;
	
	substrLine << line;
	while (std::getline(substrLine, output, ' '))
		faces.push_back(output);

	for (std::string& item : faces)
	{
		unsigned int outputFace;
		std::stringstream substrFace;
		unsigned int i = 0;

		substrFace << item;
		while (std::getline(substrFace, output, '/'))
			facesIndex.push_back(stof(output));
	}
	return;
}

static void ParseMTLFile(std::string const& fileName, std::vector<Engine::Graphics::Material>& materials)
{
	if (File::Exists(fileName))
	{
		File mtl(fileName);
		std::string line;
		Engine::Graphics::Material* currentMaterial = nullptr;

		while (mtl.ReadLine(line) != 0)
		{
			if ( ! line.empty() )
				line = Engine::Helpers::Trim(line);

			if (line.compare(0, 6, "newmtl") == 0)
			{
				materials.resize(materials.size() + 1);
				currentMaterial = &materials.back();
				currentMaterial->Name = line.substr(7, line.length() - 7);
			}

			else if (line.compare(0, 2, "Ka") == 0)
			{
				Engine::Maths::Vector3 ambient = {};
				
				line = line.substr(3, line.length() - 3);
				ExtractVector3(line, ambient);
				if (currentMaterial != nullptr)
					currentMaterial->Ambient = ambient;
			}

			else if (line.compare(0, 2, "Kd") == 0)
			{
				Engine::Maths::Vector3 diffuse = {};

				line = line.substr(3, line.length() - 3);
				ExtractVector3(line, diffuse);
				if (currentMaterial != nullptr)
					currentMaterial->Diffuse = diffuse;
			}

			else if (line.compare(0, 2, "Ks") == 0)
			{
				Engine::Maths::Vector3 specular = {};

				line = line.substr(3, line.length() - 3);
				ExtractVector3(line, specular);
				if (currentMaterial != nullptr)
					currentMaterial->Specular = specular;
			}

			else if (line.compare(0, 2, "Ns") == 0)
			{
				line = line.substr(3, line.length() - 3);
				if (currentMaterial != nullptr)
					ExtractFloat(line, currentMaterial->SpecularExponent);
			}

			else if (line.compare(0, 2, "d ") == 0)
			{
				line = line.substr(2, line.length() - 2);
				if (currentMaterial != nullptr)
					ExtractFloat(line, currentMaterial->Opacity);
			}

			else if (line.compare(0, 5, "illum") == 0)
			{
				line = line.substr(6, line.length() - 6);
				if (currentMaterial != nullptr)
					ExtractFloat(line, currentMaterial->Illumination);
			}

			else if (line.compare(0, 6, "map_Ka") == 0)
			{
				std::string texPath = fileName.substr(0, fileName.find_last_of('/') + 1) + line.substr(7, line.length() - 7);
				if (currentMaterial != nullptr)
					currentMaterial->AmbientTexture = new Engine::Graphics::Texture(texPath, Engine::Graphics::TextureType::TEXTURE_2D);
			}

			else if (line.compare(0, 6, "map_Kd") == 0)
			{
				std::string texPath = fileName.substr(0, fileName.find_last_of('/') + 1) + line.substr(7, line.length() - 7);
				if (currentMaterial != nullptr)
					currentMaterial->DiffuseTexture = new Engine::Graphics::Texture(texPath, Engine::Graphics::TextureType::TEXTURE_2D);
			}

			else if (line.compare(0, 6, "map_Ks") == 0)
			{
				std::string texPath = fileName.substr(0, fileName.find_last_of('/') + 1) + line.substr(7, line.length() - 7);
				if (currentMaterial != nullptr)
					currentMaterial->SpecularTexture = new Engine::Graphics::Texture(texPath, Engine::Graphics::TextureType::TEXTURE_2D);
			}
		}
	}
}

Engine::Graphics::Model Loader<ModelType::OBJ>::LoadModel(std::string const& modelPath)
{
	Engine::Graphics::Model toReturn;
	Engine::Graphics::Mesh* currentMeshGroup = nullptr;

	toReturn.SetName(modelPath.substr(modelPath.find_last_of('/') + 1,
					modelPath.find_last_of('.') - (modelPath.find_last_of('/') + 1)));
	if (Engine::Filesystem::File::Exists(modelPath))
	{
		std::vector<Engine::Maths::Vector3> vertices;
		std::vector<Engine::Maths::Vector2> textures;
		std::vector<Engine::Maths::Vector3> normals;
		std::vector<Engine::Maths::Vector3> paramSpace;
		std::vector<unsigned int> faces;
		std::vector<Engine::Graphics::Material> materials;

		Engine::Filesystem::File model(modelPath);
		std::string line;

		_mBasePath = modelPath.substr(0, modelPath.find_last_of('/') + 1);
		while (model.ReadLine(line) != 0)
		{
			if (line.compare(0, 6, "mtllib") == 0)
			{
				// Create material object from MTL file
				std::string mtlPath = _mBasePath + line.substr(7, line.length() - 7);
				ParseMTLFile(mtlPath, materials);
			}

			else if (line.compare(0, 6, "usemtl") == 0)
			{
				if (materials.size() > 0)
				{
					std::string matName = line.substr(7, line.length() - 7);
					auto matIt = std::find_if(materials.begin(), materials.end(), [&matName](const auto& item) {
							return matName.compare(item.Name) == 0;
						});
					if (matIt != materials.end())
					{
						currentMeshGroup->Mat = new Engine::Graphics::Material(*matIt);
					}
				}
			}

			else if (line.compare(0, 2, "g ") == 0
				/*|| line.compare(0, 2, "o ") == 0*/)
			{
				currentMeshGroup = toReturn.CreateMesh();
				currentMeshGroup->GroupName = line.substr(2, line.length() - 2);
			}

			else if (line.compare(0, 2, "v ") == 0)
			{
				Engine::Maths::Vector3 vert;

				line = line.substr(2, line.length() - 2);
				ExtractVector3(line, vert);
				vertices.push_back(vert);
			}

			else if (line.compare(0, 2, "f ") == 0)
			{
				std::vector<unsigned int> face;

				line = line.substr(2, line.length() - 2);
				if (currentMeshGroup == nullptr)
				{
					currentMeshGroup = toReturn.CreateMesh();
					currentMeshGroup->GroupName = "Default";
				}

				ExtractFaces(line, face);
				unsigned int iFace = 0;
				for (unsigned int index : face)
				{
					iFace %= 3;

					switch (iFace++)
					{
					case 0:
						currentMeshGroup->Vertices.push_back(vertices.at(
							static_cast<int>(index) - 1
						));
						break;
					case 1:
					{
						std::vector<Engine::Maths::Vector3> coords;

						coords.push_back(textures.at(
							static_cast<int>(index) - 1
						));
						currentMeshGroup->TextureCoords.push_back(coords);
					}
						break;
					case 2:
						currentMeshGroup->Normals.push_back(normals.at(
							static_cast<int>(index) - 1
						));
						break;
					}
				}
			}

			else if (line.compare(0, 2, "vt") == 0)
			{
				Engine::Maths::Vector2 texCoord;

				line = line.substr(3, line.length() - 3);
				ExtractVector2(line, texCoord);
				textures.push_back(texCoord);
			}

			else if (line.compare(0, 2, "vn") == 0)
			{
				Engine::Maths::Vector3 normal;

				line = line.substr(3, line.length() - 3);
				ExtractVector3(line, normal);
				normals.push_back(normal);
			}

			else if (line.compare(0, 2, "vp") == 0)
			{
				Engine::Maths::Vector3 space;

				line = line.substr(3, line.length() - 3);
				ExtractVector3(line, space);
				paramSpace.push_back(space);
			}
		}
	}
	toReturn.UpdateModelData();
	return toReturn;
}
