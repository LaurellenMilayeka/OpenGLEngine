#include "Loader.h"
#include "Image.h"
#include "Material.h"
#include "Vector4.h"
#include "TextureManager.h"

#include <pugixml.hpp>
#include <sstream>
#include <cstring>

using namespace Engine::InternalIO;

std::string Loader<ModelType::DAE>::_mBasePath;

typedef struct {
	std::string MaterialID;
	std::string BoundMaterialID;
} Node;

typedef struct {
	std::string SourceID;
	std::string SourceName;
	unsigned int Count;
	unsigned int Stride;
	std::vector<Engine::Maths::Vector3> Vec3;
	std::vector<Engine::Maths::Vector4> Vec4;
} Source;

typedef struct {
	std::string Semantic;
	std::string SourceID;
	unsigned int Offset;
	unsigned int Set;
} Input;

typedef struct {
	std::vector<Input> Inputs;
} Vertice;

typedef struct {
	std::string MaterialID;
	unsigned int Count;
	std::vector<Input> Inputs;
	std::vector<unsigned int> VertexCount;
	std::vector<unsigned int> Indices;
} Triangle;

typedef struct {
	std::string MeshName;
	std::vector<Source> Sources;
	Vertice Vertices;
	Triangle Triangles;
} Mesh;

typedef struct {
	std::string EffectID;
	std::string ImageID;
	Engine::Maths::Vector4 Emission;
	Engine::Maths::Vector4 Ambient;
	Engine::Maths::Vector4 Specular;
	Engine::Maths::Vector4 Transparent;
	float Shininess;
	float Transparency;
	float IndexOfRefraction;

} Effect;

typedef struct{
	std::string ImageID;
	std::string ImagePath;
} Image;

typedef struct {
	std::string MaterialID;
	std::string MaterialName;
	std::string EffectID;
} Material;

enum EffectData
{
	UNDEFINED = 0,
	EMISSION,
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	SHININESS,
	TRANSPARENT,
	TRANSPARENCY,
	REFRACTION
};

std::string GetAttribute(std::string const& line, std::string const& attribute) {
	size_t indStart;
	size_t indEnd;

	indStart = line.find(attribute + "=\"") + (attribute.length() + 2);
	indEnd = line.find("\"", indStart);
	return line.substr(indStart, indEnd - indStart);
}

std::string GetValue(std::string const& line, std::string const& nodeName) {
	return "";
}

void LoadEffects(Engine::Filesystem::File& file, std::string& line, std::vector<Effect>& effects) {
	Effect* currentEffect = nullptr;
	size_t indStart;
	size_t indEnd;

	while (file.ReadLine(line)
		&& line.find("</library_effects>") == std::string::npos) {
		if (line.find("<effect") != std::string::npos) {
			std::string effectName = GetAttribute(line, "id");

			effects.resize(effects.size() + 1);
			currentEffect = &effects.back();
			currentEffect->EffectID = effectName;
		}
		else if (line.find("<init_from>") != std::string::npos) {
			indStart = line.find("<init_from>") + 11;
			indEnd = line.find("</", indStart);
			std::string imageId = line.substr(indStart, indEnd - indStart);

			currentEffect->ImageID = imageId;
		}
		else if (line.find("<technique") != std::string::npos)
		{
		}
	}
	return;
}

void LoadImages(Engine::Filesystem::File& file, std::string& line, std::vector<Image>& images) {
	Image* currentImage = nullptr;
	size_t indStart;
	size_t indEnd;

	while (file.ReadLine(line)
		&& line.find("</library_images>") == std::string::npos) {
		if (line.find("<image ") != std::string::npos) {
			std::string imageName = GetAttribute(line, "id");

			images.resize(images.size() + 1);
			currentImage = &images.back();
			currentImage->ImageID = imageName;
		}
		else if (line.find("<init_from>") != std::string::npos) {
			indStart = line.find("<init_from>") + 11;
			indEnd = line.find("</", indStart);
			std::string imagePath = line.substr(indStart, indEnd - indStart);

			currentImage->ImagePath = imagePath;
		}
	}
	return;
}

void LoadMaterials(Engine::Filesystem::File& file, std::string& line, std::vector<Material>& materials) {
	Material *currentMaterial = nullptr;

	while (file.ReadLine(line)
		&& line.find("</library_materials>") == std::string::npos) {
		if (line.find("<material") != std::string::npos) {
			materials.resize(materials.size() + 1);
			currentMaterial = &materials.back();
			currentMaterial->MaterialName = GetAttribute(line, "name");
			currentMaterial->MaterialID = GetAttribute(line, "id");
		}
		else if (line.find("<instance_effect") != std::string::npos) {
			std::string effectName = GetAttribute(line, "url").substr(1);

			currentMaterial->EffectID = effectName;
		}
	}
	return;
}

void LoadSources(Engine::Filesystem::File& file, std::string line, Mesh* mesh) {
	Source* currentSource = nullptr;

	if (line.find("<source") != std::string::npos) {
		std::string sourceId = GetAttribute(line, "id");
		std::string sourceName = GetAttribute(line, "name");

		mesh->Sources.resize(mesh->Sources.size() + 1);
		currentSource = &mesh->Sources.back();
		currentSource->SourceID = sourceId;
		currentSource->SourceName = sourceName;
		while (file.ReadLine(line)
			&& line.find("</source>") == std::string::npos) {
			if (line.find("<float_array") != std::string::npos) {
				currentSource->Count = stoi(GetAttribute(line, "count"));
				line = line.substr(line.find('>') + 1, line.length() - (line.find('>') + 1));
				line = line.substr(0, line.find('<'));
				std::stringstream values(line);

				if (line.find("positions") != std::string::npos) {
					for (unsigned int index = 0; index < currentSource->Count / 3; index++) {
						Engine::Maths::Vector3 vertex;

						values >> vertex.x >> vertex.y >> vertex.z;
						currentSource->Vec3.push_back(vertex);
					}
				}
				else if (line.find("map1") != std::string::npos) {
					for (unsigned int index = 0; index < (currentSource->Count / 2); index++) {
						Engine::Maths::Vector2 tex;

						values >> tex.x >> tex.y;
						currentSource->Vec3.push_back(tex);
					}
				}
				else
				{
					while (file.ReadLine(line)
						&& line.find("<accessor") == std::string::npos);
					currentSource->Stride = stoi(GetAttribute(line, "stride"));

					Engine::Maths::Vector3 data;
					for (unsigned int index = 0; index < (currentSource->Count / currentSource->Stride); index++) {
						switch (currentSource->Stride)
						{
						case 2:
						{
							values >> data.x >> data.y;
							currentSource->Vec3.push_back({ data.x, data.y, 0.0f });
						}
						break;
						case 3:
						{
							values >> data.x >> data.y >> data.z;
							currentSource->Vec3.push_back(data);
						}
						break;
						}
					}
				}
			}
		}
	}
	return;
}

void LoadVertices(Engine::Filesystem::File& file, std::string line, Mesh* mesh) {
	if (line.find("<vertices") != std::string::npos) {
		while (file.ReadLine(line)
			&& line.find("</vertices>") == std::string::npos)
		{
			std::string dataType = GetAttribute(line, "semantic");
			std::string id = GetAttribute(line, "source").substr(1);
			Input input({ dataType, id, 0, 0 });

			mesh->Vertices.Inputs.push_back(input);
		}
	}
	return;
}

void LoadTriangles(Engine::Filesystem::File& file, std::string line, Mesh* mesh) {
	std::string materialID, count;

	if (line.find("<triangles") != std::string::npos) {
		mesh->Triangles.MaterialID = GetAttribute(line, "material");
		mesh->Triangles.Count = stoi(GetAttribute(line, "count"));
		while (file.ReadLine(line)
			&& line.find("</triangles>") == std::string::npos)
		{
			if (line.find("<input") != std::string::npos)
			{
				Input* currentInput = nullptr;
				std::string semantic = GetAttribute(line, "semantic");
				std::string offset   = GetAttribute(line, "offset"  );
				std::string source   = GetAttribute(line, "source"  ).substr(1);
				std::string set      = GetAttribute(line, "set"     );

				mesh->Triangles.Inputs.resize(mesh->Triangles.Inputs.size() + 1);
				currentInput = &mesh->Triangles.Inputs.back();

				currentInput->Semantic = semantic;
				currentInput->Offset   = std::stoi(offset);
				currentInput->SourceID = source;
				currentInput->Set      = std::stoi(set);
			}
			else if (line.find("<p>") != std::string::npos)
			{
				unsigned int index = 0;
				line = line.substr(line.find('>') + 1, line.length() - (line.find('>') + 1));
				line = line.substr(0, line.find('<'));

				std::stringstream indicesValues(line);
				while (indicesValues >> index)
				{
					mesh->Triangles.Indices.push_back(index);
				}
			}
		}
	}
	return;
}

void LoadPolylist(Engine::Filesystem::File& file, std::string line, Mesh* mesh) {
	std::string materialID, count;

	if (line.find("<polylist") != std::string::npos) {
		mesh->Triangles.MaterialID = GetAttribute(line, "material");
		mesh->Triangles.Count = stoi(GetAttribute(line, "count"));
		while (file.ReadLine(line)
			&& line.find("</polylist>") == std::string::npos)
		{
			if (line.find("<input") != std::string::npos)
			{
				Input* currentInput = nullptr;
				std::string semantic = GetAttribute(line, "semantic");
				std::string offset = GetAttribute(line, "offset");
				std::string source = GetAttribute(line, "source").substr(1);
				std::string set = GetAttribute(line, "set");

				mesh->Triangles.Inputs.resize(mesh->Triangles.Inputs.size() + 1);
				currentInput = &mesh->Triangles.Inputs.back();

				currentInput->Semantic = semantic;
				currentInput->Offset = std::stoi(offset);
				currentInput->SourceID = source;
				currentInput->Set = std::stoi(set);
			}
			else if (line.find("<vcount>") != std::string::npos)
			{
				unsigned int index = 0;
				line = line.substr(line.find('>') + 1, line.length() - (line.find('>') + 1));
				line = line.substr(0, line.find('<'));

				std::stringstream vertexCount(line);
				while (vertexCount >> index)
				{
					mesh->Triangles.VertexCount.push_back(index);
				}
			}
			else if (line.find("<p>") != std::string::npos)
			{
				unsigned int index = 0;
				line = line.substr(line.find('>') + 1, line.length() - (line.find('>') + 1));
				line = line.substr(0, line.find('<'));

				std::stringstream indicesValues(line);
				while (indicesValues >> index)
				{
					mesh->Triangles.Indices.push_back(index);
				}
			}
		}
	}
	return;
}

void LoadGeometries(Engine::Filesystem::File& file, std::vector<Mesh>& meshes) {
	Mesh* currentMesh = nullptr;
	std::string line;

	while (file.ReadLine(line)
		&& line.find("</library_geometries>") == std::string::npos) {
		if (line.find("<geometry") != std::string::npos) {
			meshes.resize(meshes.size() + 1);
			currentMesh = &meshes.back();
			currentMesh->MeshName = GetAttribute(line, "name");
			while (file.ReadLine(line)
				&& line.find("</mesh>") == std::string::npos) {
				if (line.find("<source") != std::string::npos) {
					LoadSources(file, line, currentMesh);
				}
				else if (line.find("<vertices") != std::string::npos) {
					LoadVertices(file, line, currentMesh);
				}
				else if (line.find("<triangles") != std::string::npos) {
					LoadTriangles(file, line, currentMesh);
				}
				else if (line.find("<polylist") != std::string::npos) {
					LoadPolylist(file, line, currentMesh);
				}
			}
		}
	}
	return;
}

void LoadVisualScenes(Engine::Filesystem::File& file, std::vector<Node>& nodes) {
	Node* currentNode = nullptr;
	std::string line;
	std::string type;

	while (file.ReadLine(line)
		&& line.find("</library_visual_scenes>") == std::string::npos) {
		if (line.find("<node") != std::string::npos) {
			type = GetAttribute(line, "type");
		}
		else if (line.find("<instance_material") != std::string::npos
			&& type.compare("JOINT") != 0) {
			nodes.resize(nodes.size() + 1);
			currentNode = &nodes.back();

			currentNode->MaterialID = GetAttribute(line, "target").substr(1);
			currentNode->BoundMaterialID = GetAttribute(line, "symbol");
		}
	}
	return;
}

void SetupModel(std::string const& basePath, Engine::Graphics::Model& model, std::vector<Mesh> const& meshes,
	std::vector<Material> const& materials, std::vector<Image> const& images,
	std::vector<Effect> const& effects, std::vector<Node> const& nodes) {
	Engine::Graphics::Mesh* currentMesh = nullptr;

	for (const Mesh& mesh : meshes) {
		currentMesh = model.CreateMesh();
		currentMesh->TextureCoords.resize(1);
		currentMesh->GroupName = mesh.MeshName;
		currentMesh->Offset    = mesh.Triangles.Inputs.back().Offset + 1;
		for (unsigned int index = 0; index < (mesh.Triangles.Count * 3 * currentMesh->Offset); index += currentMesh->Offset)
		{
			for (const Input& input : mesh.Triangles.Inputs) {
				auto itSource = std::find_if(mesh.Sources.begin(), mesh.Sources.end(), [&input](const auto& itemInput) {
					return input.SourceID.compare(itemInput.SourceID) == 0;
					});

				if (itSource == mesh.Sources.end()) {
					auto itInput = mesh.Vertices.Inputs.begin();
					for (; itInput != mesh.Vertices.Inputs.end(); itInput++) {
						itSource = std::find_if(mesh.Sources.begin(), mesh.Sources.end(), [&itInput](const auto& itemInput) {
							return itInput->SourceID.compare(itemInput.SourceID) == 0;
							});

						assert(itSource != mesh.Sources.end());

						Source foundVertSource = *itSource;

						if (itInput->Semantic.compare("VERTEX") == 0
							|| itInput->Semantic.compare("POSITION") == 0) {
							currentMesh->Vertices.push_back(foundVertSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
						}
						else if (itInput->Semantic.compare("NORMAL") == 0) {
							currentMesh->Normals.push_back(foundVertSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
						}
						else if (itInput->Semantic.compare("TEXCOORD") == 0) {
							auto itTexType = std::find_if(foundVertSource.Vec3.begin(), foundVertSource.Vec3.end(), [](const auto& item) {
								return item.z != 0;
								});

							if (itTexType != foundVertSource.Vec3.end()) {
								currentMesh->Is3DTexture = true;
							}
							else {
								currentMesh->Is3DTexture = false;
							}
							currentMesh->TextureCoords[0].push_back(foundVertSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
						}
					}
				}
				else {
					assert(itSource != mesh.Sources.end());

					Source foundSource = *itSource;

					if (input.Semantic.compare("VERTEX") == 0
						|| input.Semantic.compare("POSITION") == 0) {
						currentMesh->Vertices.push_back(foundSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
					}
					else if (input.Semantic.compare("NORMAL") == 0) {
						currentMesh->Normals.push_back(foundSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
					}
					else if (input.Semantic.compare("TEXCOORD") == 0) {
						if (foundSource.Vec3.size() > 0) {
							auto itTexType = std::find_if(foundSource.Vec3.begin(), foundSource.Vec3.end(), [](const auto& item) {
								return item.z != 0;
								});

							if (itTexType != foundSource.Vec3.end()) {
								currentMesh->Is3DTexture = true;
							}
							else {
								currentMesh->Is3DTexture = false;
							}
							currentMesh->TextureCoords[0].push_back(foundSource.Vec3.at(mesh.Triangles.Indices[index + input.Offset]));
						}
					}
				}
			}
		}

		auto itNode = std::find_if(nodes.begin(), nodes.end(), [&mesh](const auto& item) {
			return mesh.Triangles.MaterialID.compare(item.BoundMaterialID) == 0;
			});

		assert(itNode != nodes.end());

		Node tmpNode = *itNode;

		auto itMat = std::find_if(materials.begin(), materials.end(), [&tmpNode](const auto& item) {
			return tmpNode.MaterialID.compare(item.MaterialID) == 0;
			});

		assert(itMat != materials.end());

		Material tmpMaterial = *itMat;

		auto itEffect = std::find_if(effects.begin(), effects.end(), [&tmpMaterial](const auto& item) {
			return tmpMaterial.EffectID.compare(item.EffectID) == 0;
			});

		assert(itEffect != effects.end());

		Effect tmpEffect = *itEffect;

		Engine::Graphics::Material material;
		material.Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		material.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		material.Diffuse = { 0.0f, 0.0f, 0.0f, 1.0f };
		material.RefractionIndex = 1.55f;
		material.Illumination = 1.0f;
		material.Opacity = 1.0f;

		auto itImg = std::find_if(images.begin(), images.end(), [&tmpEffect](const auto& item) {
			return tmpEffect.ImageID.compare(item.ImageID) == 0;
			});

		assert(itImg != images.end());

		Image tmpImage = *itImg;
		
		//material.AmbientTexture = new Engine::Graphics::Texture(basePath + tmpImage.ImagePath);
		material.DiffuseTexture = Engine::Managers::TextureManager::CreateTexture(basePath + tmpImage.ImagePath, (currentMesh->Is3DTexture) ? Engine::Graphics::TextureType::TEXTURE_3D : Engine::Graphics::TextureType::TEXTURE_2D);
		//material.SpecularTexture = new Engine::Graphics::Texture(basePath + tmpImage.ImagePath);
		currentMesh->Mat = new Engine::Graphics::Material(material);
	}
	return;
}

Engine::Graphics::Model Loader<ModelType::DAE>::LoadModel(std::string const& modelPath) {
	Engine::Graphics::Model toReturn;
	std::vector<Effect> effects;
	std::vector<Image> images;
	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;
	Engine::Filesystem::File file(modelPath);
	std::string line;

	if (file.IsValid()) {
		_mBasePath = modelPath.substr(0, modelPath.find_last_of('/') + 1);
		while (file.ReadLine(line)) {
			if (line.find("<library_geometries>") != std::string::npos) { LoadGeometries(file, meshes); }
			else if (line.find("<library_materials>") != std::string::npos) { LoadMaterials(file, line, materials); }
			else if (line.find("<library_images>") != std::string::npos) { LoadImages(file, line, images); }
			else if (line.find("<library_effects>") != std::string::npos) { LoadEffects(file, line, effects); }
			else if (line.find("<library_visual_scenes>") != std::string::npos) { LoadVisualScenes(file, nodes); }
		}
		SetupModel(_mBasePath, toReturn, meshes, materials, images, effects, nodes);
		toReturn.UpdateModelData();
	}
	else {
		std::cout << "Could not load file " << modelPath << std::endl;
	}
	return toReturn;
}