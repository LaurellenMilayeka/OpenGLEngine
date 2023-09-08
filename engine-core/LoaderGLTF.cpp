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
	ComponentType          ComponentType;
	unsigned int           Count;
	Engine::Maths::Vector4 MaxVector;
	Engine::Maths::Matrix4 MaxMatrix;
	Engine::Maths::Vector4 MinVector;
	Engine::Maths::Matrix4 MinMatrix;
	AccessorType           Type;
};

typedef struct
{
	std::string  FileName;
	unsigned int Size;
} Buffer;

typedef struct
{
	unsigned short BufferId;
	unsigned int   Size;
	unsigned int   Offset;
	unsigned char  Stride;
	std::string    Name;
	unsigned int   Target; // RFU ?
} BufferView;

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

		if (data.HasMember("buffer"))
			bufferView.BufferId = data["buffer"    ].GetInt   ();
		if (data.HasMember("byteLength"))
			bufferView.Size     = data["byteLength"].GetInt   ();
		if (data.HasMember("byteOffset"))
			bufferView.Offset   = data["byteOffset"].GetInt   ();
		if (data.HasMember("byteStride"))
			bufferView.Stride   = data["byteStride"].GetInt   ();
		if (data.HasMember("name"))
			bufferView.Name     = data["name"      ].GetString();
		if (data.HasMember("target"))
			bufferView.Target   = data["target"    ].GetInt   ();

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
		
		if (data.HasMember("byteLength"))
			buffer.Size     = data["byteLength"].GetInt   ();
		if (data.HasMember("uri"))
			buffer.FileName = data["uri"       ].GetString();

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

		if (data.HasMember("bufferView"))
			accessor.BufferViewId = data["bufferView"].GetInt();
		if (data.HasMember("componentType"))
			accessor.ComponentType = CastDataToComponentTypeEnum(data["componentType"].GetInt());
		if (data.HasMember("count"))
			accessor.Count = data["count"].GetInt();
		if (data.HasMember("type"))
			accessor.Type = StrToAccessorTypeEnum(data["type"].GetString());

		switch (accessor.Type)
		{
		case AccessorType::Scalar:
			break;
		case AccessorType::Vector2:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector2>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector2>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		case AccessorType::Vector3:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector3>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector3>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		case AccessorType::Vector4:
			if (data.HasMember("max"))
				accessor.MaxVector = ExtractDataFromJSONArray<Engine::Maths::Vector4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinVector = ExtractDataFromJSONArray<Engine::Maths::Vector4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		case AccessorType::Matrix2:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		case AccessorType::Matrix3:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		case AccessorType::Matrix4:
			if (data.HasMember("max"))
				accessor.MaxMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["max"], GetDataSizeFromComponentType(accessor.ComponentType));
			if (data.HasMember("min"))
				accessor.MinMatrix = ExtractDataFromJSONArray<Engine::Maths::Matrix4>(data["min"], GetDataSizeFromComponentType(accessor.ComponentType));
			break;
		}
		toReturn[index++] = accessor;
	}
	return toReturn;
}

std::vector<std::string> ParseImages(Value const& images)
{
	assert(images.IsArray());
	
	unsigned int index = 0;
	std::vector<std::string> toReturn(images.Size());

	for (auto& value : images.GetArray())
	{
		assert(value.IsObject());
		GenericObject data = value.GetObject();

		if (data.HasMember("uri"))
			toReturn[index++] = data["uri"].GetString();
	}
	return toReturn;
}

std::vector<Engine::Graphics::Texture*> ParseTextures(Value const& textures, std::vector<std::string> const& images, std::string const& basePath)
{
	assert(textures.IsArray());

	unsigned int index = 0;
	std::vector<Engine::Graphics::Texture*> toReturn(textures.Size());

	for (auto& value : textures.GetArray())
	{
		assert(value.IsObject());
		GenericObject data = value.GetObject();

		//TODO : Manage sampling
		toReturn[index++] = Engine::Managers::TextureManager::CreateTexture(basePath + images[data["source"].GetInt()], Engine::Graphics::TextureType::TEXTURE_2D);
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
	std::vector<std::string> images{};
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
	textures = ParseTextures(doc["textures"], images, _mBasePath);

	int bufferFileIndex = 0;
	std::vector<const unsigned char*> bufferFiles(buffers.size());
	for (auto& buffer : buffers)
	{
		std::string out;
		Engine::Filesystem::File binFile(_mBasePath + buffer.FileName, Engine::Filesystem::File::OpenMode::Binary);
		assert(binFile.Length() == buffer.Size);

		binFile.Read(out, binFile.Length());
		bufferFiles[bufferFileIndex] = reinterpret_cast<const unsigned char*>(out.c_str());
	}
	return toReturn;
}