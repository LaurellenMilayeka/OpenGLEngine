#include "Loader.h"
#include "TextureManager.h"
#include "Logger.h"

using namespace Engine::InternalIO;

std::string Loader::_mBasePath;
std::map<std::string, unsigned int> Loader::_mBoneMapping;

void Loader::ExtractMeshes(Engine::Graphics::Mesh* mesh, std::vector<Engine::Graphics::Material*>& materialList, aiMesh const* assimpMesh, aiScene const* scene)
{
    unsigned int vertexIndex = 0;

    mesh->GroupName = assimpMesh->mName.C_Str();
    LOG_DEBUG("Processing mesh " + mesh->GroupName + "...");
    LOG_DEBUG("Found " + std::to_string(assimpMesh->mNumVertices) + " vertices for mesh " + mesh->GroupName);
    LOG_DEBUG("Processing vertices...");
    for (; vertexIndex < assimpMesh->mNumVertices; vertexIndex++)
    {
        Engine::Maths::Vector3 vertex{};

        vertex.x = assimpMesh->mVertices[vertexIndex].x;
        vertex.y = assimpMesh->mVertices[vertexIndex].y;
        vertex.z = assimpMesh->mVertices[vertexIndex].z;
        mesh->Vertices.push_back(vertex);
    }
    LOG_DEBUG(std::to_string(vertexIndex) + " vertices processed for mesh " + mesh->GroupName);

    if (assimpMesh->HasNormals())
    {
        unsigned int normalIndex = 0;

        LOG_DEBUG("Found " + std::to_string(assimpMesh->mNumVertices) + " normals for mesh " + mesh->GroupName);
        LOG_DEBUG("Processing normals...");
        for (; normalIndex < assimpMesh->mNumVertices; normalIndex++)
        {
            Engine::Maths::Vector3 normal{};

            normal.x = assimpMesh->mVertices[normalIndex].x;
            normal.y = assimpMesh->mVertices[normalIndex].y;
            normal.z = assimpMesh->mVertices[normalIndex].z;
            mesh->Normals.push_back(normal);
        }
        LOG_DEBUG(std::to_string(normalIndex) + " normals processed for mesh " + mesh->GroupName);
    }

    if (assimpMesh->HasTextureCoords(0))
    {
        unsigned int texIndex = 0;

        LOG_DEBUG("Found " + std::to_string(assimpMesh->mNumVertices) + " UV coordinates for mesh " + mesh->GroupName);
        LOG_DEBUG("Processing UV coordinates...");
        std::vector<Engine::Maths::Vector3> texCoords{};
        for (; texIndex < assimpMesh->mNumVertices; texIndex++)
        {
            Engine::Maths::Vector3 texCoord{};

            texCoord.x = assimpMesh->mTextureCoords[0][texIndex].x;
            texCoord.y = assimpMesh->mTextureCoords[0][texIndex].y;
            texCoord.z = assimpMesh->mTextureCoords[0][texIndex].z;
            texCoords.push_back(texCoord);
        }
        mesh->TextureCoords.push_back(texCoords);
        LOG_DEBUG(std::to_string(texIndex) + " UV coordinates processed for mesh " + mesh->GroupName);
    }

    if (assimpMesh->HasFaces())
    {
        unsigned int faceIndex = 0;
        std::vector<unsigned int> faces{};

        LOG_DEBUG("Found " + std::to_string(assimpMesh->mNumFaces) + " faces for mesh " + mesh->GroupName);
        LOG_DEBUG("Processing faces...");
        for (; faceIndex < assimpMesh->mNumFaces; faceIndex++)
        {
            unsigned int indiceIndex = 0;
            
            for (; indiceIndex < assimpMesh->mFaces[faceIndex].mNumIndices; indiceIndex++)
            {
                faces.push_back(assimpMesh->mFaces[faceIndex].mIndices[indiceIndex]);
            }
        }
        LOG_DEBUG(std::to_string(faceIndex) + " faces processed for mesh " + mesh->GroupName);
        mesh->Indices[Engine::Graphics::PolyType::TRIANGLES] = faces;
    }

    if (assimpMesh->HasBones())
    {
        unsigned int numBone = 0;

        LOG_DEBUG("Found " + std::to_string(assimpMesh->mNumBones) + " bones for mesh " + mesh->GroupName);
        LOG_DEBUG("Processing bones...");
        for (unsigned int index = 0; index < assimpMesh->mNumBones; index++)
        {
            unsigned int boneIndex = 0;
            std::string boneName(assimpMesh->mBones[index]->mName.data);

            if (_mBoneMapping.find(boneName) == _mBoneMapping.end()) {
                boneIndex = numBone++;
                //BoneInfo bi;
                //m_BoneInfo.push_back(bi);
            }
            else {
                boneIndex = _mBoneMapping[boneName];
            }

            LOG_DEBUG("Bone " + boneName + " processing...");
        }
    }

    if (assimpMesh->mMaterialIndex >= 0)
    {
        LOG_DEBUG("Found material index for mesh " + mesh->GroupName);
        LOG_DEBUG("Setting material...");
        mesh->Mat = materialList[assimpMesh->mMaterialIndex];
        LOG_DEBUG("Material set");
    }
}

void Loader::ExtractNodes(Engine::Graphics::Model& model, std::vector<Engine::Graphics::Material*>& materialList, aiNode* node, const aiScene* scene)
{
    Engine::Graphics::Mesh* currentMesh = nullptr;

    LOG_DEBUG("Processing node " + std::string(node->mName.C_Str()));
    // process all the node's meshes (if any)
    LOG_DEBUG("Found " + std::to_string(node->mNumMeshes) + " meshes for the current node");
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        currentMesh = model.CreateMesh();
        currentMesh->Transformation = Engine::Maths::Matrix4((float*)&scene->mRootNode->mTransformation);
        currentMesh->Transformation = Engine::Maths::Matrix4::Invert(currentMesh->Transformation);
        ExtractMeshes(currentMesh, materialList, scene->mMeshes[node->mMeshes[i]], scene);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ExtractNodes(model, materialList, node->mChildren[i], scene);
    }
}

void Loader::ExtractMaterials(std::vector<Engine::Graphics::Material*>& materialList, std::string const& basePath, const aiScene* scene)
{
    Engine::Graphics::Material* currentMat = nullptr;

    materialList.resize(scene->mNumMaterials);
    LOG_DEBUG("Found " + std::to_string(scene->mNumMaterials) + " materials.");
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];

        currentMat = new Engine::Graphics::Material();
        currentMat->Name = material->GetName().C_Str();
        LOG_DEBUG("Processing material " + currentMat->Name + "...");
        LOG_DEBUG("Fetching DIFFUSE textures for material " + currentMat->Name + "...");
        LOG_DEBUG("Found " + std::to_string(material->GetTextureCount(aiTextureType_DIFFUSE)) + " DIFFUSE textures");
        for (unsigned int texCount = 0; texCount < material->GetTextureCount(aiTextureType_DIFFUSE); texCount++)
        {
            aiString texPath;
            material->GetTexture(aiTextureType_DIFFUSE, texCount, &texPath);
            currentMat->DiffuseTexture = Engine::Managers::TextureManager::CreateTexture(basePath + texPath.C_Str(), Engine::Graphics::TextureType::TEXTURE_2D);
        }
        materialList[i] = currentMat;
        LOG_DEBUG("Saved " + currentMat->Name);
    }
}

Engine::Graphics::Model Loader::LoadModel(std::string const& modelPath)
{
    Assimp::Importer import{};
    Engine::Graphics::Model modelToReturn{};
    std::vector<Engine::Graphics::Material*> materialList;
    
    LOG_INFO("Loading model : " + modelPath);
    
    modelToReturn.SetName(modelPath.substr(modelPath.find_last_of('/') + 1,
        modelPath.find_last_of('.') - (modelPath.find_last_of('/') + 1)));

    const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR(import.GetErrorString());
        return {};
    }
    _mBasePath = modelPath.substr(0, modelPath.find_last_of('/') + 1);
    LOG_DEBUG("Base path for model is : " + _mBasePath);
    LOG_INFO("Extracting Materials");

    ExtractMaterials(materialList, _mBasePath, scene);
    LOG_INFO("Extracting Nodes");
    ExtractNodes(modelToReturn, materialList, scene->mRootNode, scene);

    if (scene->HasAnimations())
    {
        LOG_INFO("Extracting Animations");
    }
    else
    {
        LOG_WARN("No animation found in this model");
    }

    modelToReturn.UpdateModelData();
    return modelToReturn;
}