#include "Loader.h"
#include "File.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "TextureManager.h"

#include "document.h"

using namespace Engine::InternalIO;
using namespace rapidjson;

std::string Loader<ModelType::GLTF>::_mBasePath;

enum class AccessorType
{
	Scalar,
	Vector2,
	Vector3,
	Vector4,
	Matrix2,
	Matrix3,
	Matrix4,

	Undefined
};

enum class ComponentType
{
	SignedByte    = 5120,
	UnsignedByte  = 5121,
	SignedShort   = 5122,
	UnsignedShort = 5123,
	UnsignedInt   = 5125,
	Float         = 5126,

	Undefined
};

struct Accessor
{
	unsigned int           BufferViewId;
	unsigned int           ByteOffset;
	ComponentType          ComponentTypeId;
	bool                   Normalized;
	unsigned int           Count;
	AccessorType           Type;
	Engine::Maths::Vector4 MaxVector;
	Engine::Maths::Matrix4 MaxMatrix;
	Engine::Maths::Vector4 MinVector;
	Engine::Maths::Matrix4 MinMatrix;
	std::string            Name;
};

struct Buffer
{
	std::string  Name;
	std::string  URI;
	unsigned int ByteLength;
};

struct BufferView
{
	unsigned short Buffer;
	unsigned int   ByteLength;
	unsigned int   ByteOffset;
	unsigned char  ByteStride;
	std::string    Name;
	unsigned int   Target; // RFU ?
};

struct Image
{
	std::string  URI;
	std::string  MIMEType;
	unsigned int BufferViewId;
	std::string  Name;
};

unsigned int GetDataSizeFromComponentType(ComponentType const componentType)
{
	unsigned int toReturn = 0;

	switch (componentType)
	{
	case ComponentType::SignedByte:
	case ComponentType::UnsignedByte:
		toReturn = 1;
		break;
	case ComponentType::SignedShort:
	case ComponentType::UnsignedShort:
		toReturn = 2;
		break;
	case ComponentType::UnsignedInt:
	case ComponentType::Float:
		toReturn = 4;
		break;
	}
	return toReturn;
}

template<typename T>
T ExtractDataFromJSONArray(Value const& arr, unsigned int dataSize)
{
	T toReturn {};
	unsigned int  index = 0;
	float values[sizeof(T) / 4];

	memset(values, 0, sizeof(values));
	for (auto& value : arr.GetArray())
		values[index++] = value.GetFloat();
	memcpy(&toReturn, &values, sizeof(values));
	return toReturn;
}

std::vector<BufferView> ParseBufferView(Value const& bufferViews)
{
	assert(bufferViews.IsArray());

	unsigned int index = 0;
	std::vector<BufferView> toReturn(bufferViews.Size());

	for (auto& value : bufferViews.GetArray())
	{
		assert(value.IsObject());

		BufferView    bufferView {};
		GenericObject data  = value.GetObject();

		// No checks needed.
		// Mandatory values as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		bufferView.Buffer         = data["buffer"    ].GetInt   ();
		bufferView.ByteLength     = data["byteLength"].GetInt   ();

		// Mandatory check necessary
		// Values could and could not be present in the accessor object
		// Marked as optional as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		if (data.HasMember("byteOffset"))
			bufferView.ByteOffset = data["byteOffset"].GetInt   ();
		if (data.HasMember("byteStride"))
			bufferView.ByteStride = data["byteStride"].GetInt   ();
		if (data.HasMember("name"))
			bufferView.Name       = data["name"      ].GetString();
		if (data.HasMember("target"))
			bufferView.Target     = data["target"    ].GetInt   ();
		
		toReturn[index++] = bufferView;
	}
	return toReturn;
}

std::vector<Buffer> ParseBuffers(Value const& buffers)
{
	assert(buffers.IsArray());

	unsigned int index = 0;
	std::vector<Buffer> toReturn(buffers.Size());

	for (auto& value : buffers.GetArray())
	{
		assert(value.IsObject());

		Buffer        buffer {};
		GenericObject data    = value.GetObject();
		
		// No checks needed.
		// Mandatory values as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		buffer.ByteLength = data["byteLength"].GetInt   ();
		
		// Mandatory check necessary
		// Values could and could not be present in the accessor object
		// Marked as optional as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		if (data.HasMember("uri"))
			buffer.URI    = data["uri"       ].GetString();
		if (data.HasMember("name"))
			buffer.Name   = data["name"      ].GetString();

		toReturn[index++] = buffer;
	}
	return toReturn;
}

AccessorType StrToAccessorTypeEnum(std::string const& strType)
{
	AccessorType toReturn = AccessorType::Undefined;

	     if (strType.compare("SCALAR") == 0) toReturn = AccessorType::Scalar ;
	else if (strType.compare("VEC2"  ) == 0) toReturn = AccessorType::Vector2;
	else if (strType.compare("VEC3"  ) == 0) toReturn = AccessorType::Vector3;
	else if (strType.compare("VEC4"  ) == 0) toReturn = AccessorType::Vector4;
	else if (strType.compare("MAT2"  ) == 0) toReturn = AccessorType::Matrix2;
	else if (strType.compare("MAT3"  ) == 0) toReturn = AccessorType::Matrix3;
	else if (strType.compare("MAT4"  ) == 0) toReturn = AccessorType::Matrix4;
	return toReturn;
}

ComponentType CastDataToComponentTypeEnum(unsigned int const data)
{
	ComponentType toReturn = ComponentType::Undefined;

	switch (data)
	{
	case 5120:
		toReturn = ComponentType::SignedByte;
		break;
	case 5121:
		toReturn = ComponentType::UnsignedByte;
		break;
	case 5122:
		toReturn = ComponentType::SignedShort;
		break;
	case 5123:
		toReturn = ComponentType::UnsignedShort;
		break;
	case 5125:
		toReturn = ComponentType::UnsignedInt;
		break;
	case 5126:
		toReturn = ComponentType::Float;
		break;
	}
	return toReturn;
}

std::vector<Accessor> ParseAccessors(Value const& accessors)
{
	assert(accessors.IsArray());

	unsigned int index = 0;
	std::vector<Accessor> toReturn(accessors.Size());
	
	for (auto& value : accessors.GetArray())
	{
		assert(value.IsObject());

		Accessor accessor {};
		GenericObject data = value.GetObject();

		// No checks needed.
		// Mandatory values as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		accessor.ComponentTypeId = CastDataToComponentTypeEnum(data["componentType"].GetInt());
		accessor.Count = data["count"].GetInt();
		accessor.Type = StrToAccessorTypeEnum(data["type"].GetString());

		// Mandatory check necessary
		// Values could and could not be present in the accessor object
		// Marked as optional as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		if (data.HasMember("bufferView"))
			accessor.BufferViewId = data["bufferView"].GetInt();
		if (data.HasMember("byteOffset"))
			accessor.ByteOffset = data["byteOffset"].GetInt();
		if (data.HasMember("normalized"))
			accessor.Normalized = data["normalized"].GetBool();
		if (data.HasMember("name"))
			accessor.Name = data["name"].GetString();

		switch (accessor.Type)
		{
		case AccessorType::Scalar:
			break;
		case AccessorType::Vector2:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector2>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector2>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		case AccessorType::Vector3:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector3>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector3>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		case AccessorType::Vector4:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		case AccessorType::Matrix2:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		case AccessorType::Matrix3:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		case AccessorType::Matrix4:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentTypeId));
			break;
		}

		// Not implemented yet

		if (data.HasMember("sparse"))
			std::cerr << "[glTF][Not Supported] accessor.sparse not supported yet" << std::endl;

		toReturn[index++] = accessor;
	}
	return toReturn;
}

std::vector<Image> ParseImages(Value const& images)
{
	assert(images.IsArray());
	
	unsigned int index = 0;
	std::vector<Image> toReturn(images.Size());

	for (auto& value : images.GetArray())
	{
		assert(value.IsObject());
		Image image {};
		GenericObject data = value.GetObject();

		// Mandatory check necessary
		// Values could and could not be present in the accessor object
		// Marked as optional as per v2.0 of glTF format specification
		// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html
		//

		if (data.HasMember("uri"))
			image.URI = data["uri"].GetString();
		else if (data.HasMember("bufferView"))
		{
			image.BufferViewId = data["bufferView"].GetInt();
			image.MIMEType = data["mimeType"].GetString();
		}

		if (data.HasMember("name"))
			image.Name = data["name"].GetString();

		toReturn[index++] = image;
	}
	return toReturn;
}

std::vector<Engine::Graphics::Texture*> ParseTextures(Value const& textures, std::vector<Image> const& images, std::string const& basePath)
{
	assert(textures.IsArray());

	unsigned int index = 0;
	std::vector<Engine::Graphics::Texture*> toReturn(textures.Size());

	for (auto& value : textures.GetArray())
	{
		assert(value.IsObject());
		GenericObject data = value.GetObject();

		//TODO : Manage sampling
		toReturn[index++] = Engine::Managers::TextureManager::CreateTexture(basePath + images[data["source"].GetInt()].URI, Engine::Graphics::TextureType::TEXTURE_2D);
	}
	return toReturn;
}

std::vector<Engine::Graphics::Material> ParseMaterials(Value const& materials)
{
	assert(materials.IsArray());

	unsigned int index = 0;
	std::vector<Engine::Graphics::Material> toReturn(materials.Size());

	for (auto& value : materials.GetArray())
	{
		assert(value.IsObject());
		Engine::Graphics::Material material {};
		GenericObject data = value.GetObject();

		if (data.HasMember("doubleSided"))
			material.IsDoubleSided = data["doubleSided"].GetBool();
		if (data.HasMember("name"))
			material.Name = data["name"].GetString();
		
		/*if (data.HasMember("metallicFactor"))
			material.Metallic = data["metallicFactor"].GetFloat();
		if (data.HasMember("roughnessFactor"))
			material.Roughness = data["roughnessFactor"].GetFloat();*/
	}
	return toReturn;
}

Engine::Graphics::Model Loader<ModelType::GLTF>::LoadModel(std::string const& modelPath)
{
	Engine::Graphics::Model toReturn{};
	Engine::Filesystem::File gltfFile(modelPath);
	std::vector<Buffer> buffers{};
	std::vector<BufferView> bufferViews{};
	std::vector<Accessor> accessors{};
	std::vector<Image> images{};
	std::vector<Engine::Graphics::Texture*> textures{};
	std::string fileContent;
	Document doc{};

	_mBasePath = modelPath.substr(0, modelPath.find_last_of('/') + 1);
	gltfFile.Read(fileContent, gltfFile.Length());
	doc.Parse(fileContent.c_str());

	buffers = ParseBuffers(doc["buffers"]);
	bufferViews = ParseBufferView(doc["bufferViews"]);
	accessors = ParseAccessors(doc["accessors"]);
	images = ParseImages(doc["images"]);
	//textures = ParseTextures(doc["textures"], images, _mBasePath);

	int bufferFileIndex = 0;
	std::vector<const unsigned char*> bufferFiles(buffers.size());
	for (auto& buffer : buffers)
	{
		std::string out;
		Engine::Filesystem::File binFile(_mBasePath + buffer.URI, Engine::Filesystem::File::OpenMode::Binary);
		assert(binFile.Length() == buffer.ByteLength);

		binFile.Read(out, binFile.Length());
		bufferFiles[bufferFileIndex] = reinterpret_cast<const unsigned char*>(out.c_str());
	}
	return toReturn;
}