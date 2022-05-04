#include "Loader.h"
#include "Image.h"
#include "Material.h"
#include "Vector4.h"

#include <pugixml.hpp>
#include <sstream>
#include <cstring>

using namespace Engine::InternalIO;

std::string Loader<ModelType::DAE>::_mBasePath;

typedef struct {
	Engine::Maths::Vector4 ambient;
	Engine::Maths::Vector4 diffuse;
	Engine::Maths::Vector4 specular;
	float refraction = 0.0f;
} Effect;

static bool HasChildren(pugi::xml_node const node)
{
	int n = 0;

	for (pugi::xml_node child : node.children()) n++;
	return n != 0;
}

static bool ParseLibraryEffects(pugi::xml_node const& effects, std::map<std::string, Effect>& effectLibrary, std::string const& basePath)
{
	if (HasChildren(effects))
	{
		for (pugi::xml_node effect = effects.first_child(); effect; effect = effect.next_sibling())
		{
			Effect newEffect;
			pugi::xml_node materialNode = effect.first_child()
												.first_child()
												.first_child();
			for (pugi::xml_node parse_effect = materialNode.first_child(); parse_effect; parse_effect = parse_effect.next_sibling())
			{
				pugi::xml_node effectValue = parse_effect.first_child();
				std::stringstream buffer;
				
				buffer << effectValue.text().as_string();
				if (std::strcmp("ambient", parse_effect.name()) == 0)
				{
					buffer >> newEffect.ambient.x >> newEffect.ambient.y >> newEffect.ambient.z >> newEffect.ambient.w;
				}
				else if (std::strcmp("diffuse", parse_effect.name()) == 0)
				{
					buffer >> newEffect.diffuse.x >> newEffect.diffuse.y >> newEffect.diffuse.z >> newEffect.diffuse.w;
				}
				else if (std::strcmp("specular", parse_effect.name()) == 0)
				{
					buffer >> newEffect.specular.x >> newEffect.specular.y >> newEffect.specular.z >> newEffect.specular.w;
				}
				else if (std::strcmp("index_of_refraction", parse_effect.name()) == 0)
				{
					buffer >> newEffect.refraction;
				}
			}
			effectLibrary[effect.attribute("id").value()] = newEffect;
		}
	}
	else
	{
		std::cout << "Node " << effects.name() << " has no children" << std::endl;
	}
	return true;
}

static bool ParseLibraryImages(pugi::xml_node const& images, std::map<std::string, Effect>&, std::string const& basePath)
{
	if (HasChildren(images))
	{

	}
	else
	{
		std::cout << "Node " << images.name() << " has no children" << std::endl;
	}
	return true;
}

static bool ParseLibraryMaterials(pugi::xml_node const& materials, std::map<std::string, Engine::Graphics::Material>& libraryMaterials,
								std::map<std::string, Effect>& libraryEffects, std::string const& basePath)
{
	if (HasChildren(materials))
	{
		for (pugi::xml_node material = materials.first_child(); material; material = material.next_sibling())
		{
			Engine::Graphics::Material newMaterial = {};
			pugi::xml_node instance = material.first_child();
			std::string url = (instance.attribute("url").value() + 1);

			newMaterial.Name    = material.attribute("name").value();
			newMaterial.Ambient = libraryEffects[url].ambient;
			newMaterial.Diffuse = libraryEffects[url].diffuse;
			newMaterial.Specular = libraryEffects[url].specular;
			newMaterial.RefractionIndex = libraryEffects[url].refraction;

			libraryMaterials[material.attribute("id").value()] = newMaterial;
		}
	}
	else
	{
		std::cout << "Node " << materials.name() << " has no children" << std::endl;
	}
	return true;
}

static bool ParseLibraryGeometries(pugi::xml_node const& geometries, std::map<std::string, Engine::Graphics::Mesh>&, std::string const& basePath)
{
	if (HasChildren(geometries) > 0)
	{
		for (pugi::xml_node geometry = geometries.first_child(); geometry; geometry = geometry.next_sibling())
		{

		}
	}
	else
	{
		std::cout << "Node " << geometries.name() << " has no children" << std::endl;
	}
	return true;
}

Engine::Graphics::Model Loader<ModelType::DAE>::LoadModel(std::string const& modelPath)
{
	pugi::xml_document doc;
	Engine::Graphics::Model toReturn;

	std::map<std::string, Effect> effects;
	std::map<std::string, Effect> images;
	std::map<std::string, Engine::Graphics::Material> materials;
	std::map<std::string, Engine::Graphics::Mesh> meshes;

	if (Engine::Filesystem::File::Exists(modelPath))
	{
		pugi::xml_parse_result result = doc.load_file(modelPath.c_str());
		pugi::xml_node rootNode = doc.first_child().first_child();

		_mBasePath = modelPath.substr(0, modelPath.find_last_of('/') + 1);
		for (pugi::xml_node node = rootNode; node; node = node.next_sibling())
		{
			if (std::strcmp("library_effects", node.name()) == 0)
			{
				ParseLibraryEffects(node, effects, _mBasePath);
			}
			else if (std::strcmp("library_images", node.name()) == 0)
			{
				ParseLibraryImages(node, images, _mBasePath);
			}
			else if (std::strcmp("library_materials", node.name()) == 0)
			{
				ParseLibraryMaterials(node, materials, effects, _mBasePath);
			}
			else if (std::strcmp("library_geometries", node.name()) == 0)
			{
				ParseLibraryGeometries(node, meshes, _mBasePath);
			}
		}
	}
	return toReturn;
}