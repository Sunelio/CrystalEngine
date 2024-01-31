#include "Resources/AssimpParser.h"
#include "Core/Engine.h"
#include "Resources/ResourceManager.h"
#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Resources/Texture.h"
#include "Resources/Material.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Debug/Logger.h"
#include "Maths/Maths.h"
#include <map>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace fs = std::filesystem;
using namespace Resources;
using namespace Maths;
using namespace Anim;

#pragma region Helpers
constexpr unsigned int ASSIMP_READ_FLAGS = aiProcess_Triangulate           |
                                           aiProcess_SortByPType           |
                                           aiProcess_GenUVCoords           |
                                           aiProcess_OptimizeMeshes        |
                                           aiProcess_ValidateDataStructure |
                                           aiProcess_ImproveCacheLocality  |
                                           aiProcess_GenNormals            |
                                           aiProcess_CalcTangentSpace      |
                                           aiProcess_LimitBoneWeights      |
                                           aiProcess_FlipWindingOrder;

static Mat4 ConvertMat4x4(const aiMatrix4x4& from)
{
	return Mat4(from.a1, from.b1, from.c1, from.d1,
				from.a2, from.b2, from.c2, from.d2,
				from.a3, from.b3, from.c3, from.d3,
				from.a4, from.b4, from.c4, from.d4);
}

static Vector3 ConvertVec3(const aiVector3D& vec)
{
	return Vector3(vec.x, vec.y, vec.z);
}

static Quaternion ConvertQuaternion(const aiQuaternion& pOrientation)
{
	return Quaternion(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
}
#pragma endregion

Mesh* AssimpParser::ParseModelFile(const std::string& filename)
{
    if (!resourceManager) resourceManager = Core::Engine::Get()->GetResourceManager();
    const std::string modelPath = fs::path(filename).parent_path().string() + "/";

    // Create an importer and read the model file.
    Assimp::Importer import;
    import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_LIGHTS,     false);
    import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_CAMERAS,    false);
    const aiScene* assimpScene = import.ReadFile(filename, ASSIMP_READ_FLAGS);
    
    // Log an error if the scene isn't loaded properly.
    if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
    {
        DebugLogError(std::string("Assimp error:\n") + import.GetErrorString());
        return {};
    }

    // Get all of the necessary bones from the assimp importer.
    std::unordered_map<std::string, aiNode*> nodes = CreateNodeMap(assimpScene->mRootNode);
    std::unordered_map<std::string, std::pair<aiNode*, aiBone*>> necessaryBones = {};
    for (uint meshIdx = 0; meshIdx < assimpScene->mNumMeshes; meshIdx++)
    {
        const aiMesh* assimpMesh = assimpScene->mMeshes[meshIdx];
        for (uint boneIdx = 0; boneIdx < assimpMesh->mNumBones; boneIdx++)
        {
            aiBone* bone = assimpMesh->mBones[boneIdx];
            aiNode* node = nodes[bone->mName.C_Str()];
            necessaryBones[node->mName.C_Str()] = std::make_pair(node, bone);
        }
    }
     
    // Create a skeleton from the necessary bones.
    std::unordered_map<std::string, Bone*> bones; bones.reserve(necessaryBones.size());
    Skeleton* skeleton = new Skeleton;
    uint      boneID   = 0;
    for (auto& [name, node] : necessaryBones)
    {
        Bone* bone = new Bone(boneID, name, skeleton->GetAnimator());

        // Extract bone offset transform.
        bone->offsetMatrix = ConvertMat4x4(node.second->mOffsetMatrix);
        
        // Add the new bone to the bones map.
        bones[name] = bone;
        boneID++;
    }
    for (auto& [name, bone] : bones)
    {
        if (const aiNode* assimpNode = necessaryBones[name].first)
        {
            // Check if there is a bone in the node's parent.
            aiNode* boneParent = nullptr;
            aiNode* parent = assimpNode->mParent;
            while (parent) {
                if (bones.count(assimpNode->mParent->mName.C_Str()) > 0) {
                    boneParent = parent;
                    break;
                }
                parent = parent->mParent;
            }

            // If there is one, set it to this bone's parent.
            if (parent) {
                bone->parent = bones[boneParent->mName.C_Str()];
            }
            
            // If there isn't, make this bone root.
            else {
                skeleton->SetRootBone(bone);
            }

            // Set this bone's children.
            for (uint i = 0; i < assimpNode->mNumChildren; i++) {
                if (Bone* child = bones[assimpNode->mChildren[i]->mName.C_Str()])
                    bone->children.push_back(child);
            }
            skeleton->AddBone(bone);
        }
    }
    std::sort(skeleton->bones.begin(), skeleton->bones.end(), [](const Bone* a, const Bone* b){ return a->id < b->id; });
    skeleton->UpdateBones();

    // Parse animations.
    for (uint i = 0; i < assimpScene->mNumAnimations; i++)
    {   
        aiAnimation* assimpAnim = assimpScene->mAnimations[i];
        Animation* animation = skeleton->GetAnimator().AddAnimation(assimpAnim->mName.C_Str());
        if (i == 0) skeleton->GetAnimator().SetCurrentAnimation(assimpAnim->mName.C_Str());
        animation->duration = (float)assimpAnim->mDuration;
        animation->speed    = (float)assimpAnim->mTicksPerSecond;

        for (uint j = 0; j < assimpAnim->mNumChannels; j++)
        {
            aiNodeAnim* assimpNodeAnim = assimpAnim->mChannels[j];

            std::map<int, RawTransform> keyframes = {};

            // Position keyframes.
            for (uint k = 0; k < assimpNodeAnim->mNumPositionKeys; k++)
            {
                aiVectorKey& currPosKey = assimpNodeAnim->mPositionKeys[k];
                keyframes[(int)(currPosKey.mTime * 100)].position = ConvertVec3(currPosKey.mValue);
            }

            // Rotation keyframes.
            for (uint k = 0; k < assimpNodeAnim->mNumRotationKeys; k++)
            {
                aiQuatKey& currRotKey = assimpNodeAnim->mRotationKeys[k];
                keyframes[(int)(currRotKey.mTime * 100)].rotation = ConvertQuaternion(currRotKey.mValue);
            }

            // Scale keyframes.
            for (uint k = 0; k < assimpNodeAnim->mNumScalingKeys; k++)
            {
                aiVectorKey& currScaleKey = assimpNodeAnim->mScalingKeys[k];
                keyframes[(int)(currScaleKey.mTime * 100)].scale = ConvertVec3(currScaleKey.mValue);
            }

            std::vector<Keyframe> boneKeyframes; boneKeyframes.reserve(keyframes.size());
            for (std::map<int, RawTransform>::iterator it = keyframes.begin(); it != keyframes.end(); ++it)
                boneKeyframes.push_back(Keyframe{ it->second, (float)it->first * 0.01f });

            if (Bone* bone = bones[assimpNodeAnim->mNodeName.C_Str()])
                animation->SetBoneKeyframes(bone->id, boneKeyframes);
        }
    }
    
    // Load all of the sub-meshes from the assimp importer.
    std::vector<SubMesh*> subMeshes;
    for (uint i = 0; i < assimpScene->mNumMeshes; i++)
    {
        const aiMesh* assimpMesh = assimpScene->mMeshes[i];
        subMeshes.push_back(SubMeshFromAssimp(assimpScene, assimpMesh, i, modelPath, bones));
    }
    Mesh* mesh = resourceManager->Create<Mesh>(fs::path(filename).stem().string(), false);
    mesh->subMeshes.insert(mesh->subMeshes.begin(), subMeshes.begin(), subMeshes.end());
    mesh->SetSkeleton(skeleton);
    mesh->Load();

    return mesh;
}

std::unordered_map<std::string, aiNode*> AssimpParser::CreateNodeMap(const aiNode* assimpNode)
{
    std::unordered_map<std::string, aiNode*> children; children.reserve(assimpNode->mNumChildren);
    for (uint i = 0; i < assimpNode->mNumChildren; i++)
    {
        aiNode* child = assimpNode->mChildren[i];
        children[child->mName.C_Str()] = child;

        if (child->mNumChildren > 0)
        {
            std::unordered_map<std::string, aiNode*> childChildren = CreateNodeMap(child);
            children.insert(childChildren.begin(), childChildren.end());
        }
    }
    return children;
}

AnimatedSubMesh* AssimpParser::SubMeshFromAssimp(const aiScene* assimpScene, const aiMesh* assimpMesh, const int& meshIndex, const std::string& modelPath, std::unordered_map<std::string, Bone*>& bones)
{
    AnimatedSubMesh*  subMesh   = new AnimatedSubMesh(assimpMesh->mName.C_Str() + std::string("_") + std::to_string(meshIndex));
    Material*         material  = resourceManager->Create<Material>(std::string("mat_") + assimpMesh->mName.C_Str() + "_" + std::to_string(meshIndex));
    const aiMaterial* assimpMat = assimpScene->mMaterials[assimpMesh->mMaterialIndex];

    // Process mesh vertices and indices.
    std::vector<AnimatedVertex> vertices;
    std::vector<uint>           indices;
    for (uint i = 0; i < assimpMesh->mNumVertices; i++)
    {
        AnimatedVertex vertex;
       
        // Positions.
        if (assimpMesh->HasPositions())
        {
            vertex.pos.x = assimpMesh->mVertices[i].x;
            vertex.pos.y = assimpMesh->mVertices[i].y;
            vertex.pos.z = assimpMesh->mVertices[i].z;
        }

        // Texture coordinates.
        if (assimpMesh->HasTextureCoords(0))
        {
            vertex.uv.x = assimpMesh->mTextureCoords[0][i].x; 
            vertex.uv.y = assimpMesh->mTextureCoords[0][i].y;
        }
   
        // Normals.
        if (assimpMesh->HasNormals())
        {
            vertex.normal.x = assimpMesh->mNormals[i].x;
            vertex.normal.y = assimpMesh->mNormals[i].y;
            vertex.normal.z = assimpMesh->mNormals[i].z;
        }
        
        if (assimpMesh->HasTangentsAndBitangents())
        {
            // Tangent.
            vertex.tangent.x = assimpMesh->mTangents[i].x;
            vertex.tangent.y = assimpMesh->mTangents[i].y;
            vertex.tangent.z = assimpMesh->mTangents[i].z;
            
            // Bitangent.
            vertex.bitangent.x = assimpMesh->mBitangents[i].x;
            vertex.bitangent.y = assimpMesh->mBitangents[i].y;
            vertex.bitangent.z = assimpMesh->mBitangents[i].z;
        }
        vertices.push_back(vertex);
    }
    
    // Loop through the mesh's faces and get  the corresponding vertex indices.
    for (uint i = 0; i < assimpMesh->mNumFaces; i++)
    {
        const aiFace& face = assimpMesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Get the bone weights.
    for (uint boneIdx = 0; boneIdx < assimpMesh->mNumBones; boneIdx++)
    {
        const aiBone*         assimpBone = assimpMesh->mBones[boneIdx];
        const uint            boneID     = bones[assimpBone->mName.C_Str()]->id;
        const aiVertexWeight* weights    = assimpBone->mWeights;
        for (uint weightIdx = 0; weightIdx < assimpBone->mNumWeights; weightIdx++)
        {
            const uint  vertexId = weights[weightIdx].mVertexId;
            const float weight   = weights[weightIdx].mWeight;
            if (vertexId >= (uint)vertices.size()) { continue; }
            
            AnimatedVertex& vertex = vertices[vertexId];
            for (int i = 0; i < 4; i++)
            {
                if (vertex.boneIDs[i] < 0)
                {
                    vertex.boneIDs[i] = boneID;
                    vertex.weights[i] = weight;
                    break;
                }
            }
        }
    }

    // Normalize bone weights.
    for (auto& vertex : vertices)
    {
        float sum = 0;
        for (int i = 0; i < 4; i++) sum += vertex.weights[i];
        for (int i = 0; i < 4; i++) vertex.weights[i] /= sum;
    }
    
    // Process material colors.
    material->ambient   = GetAssimpMaterialColor(assimpMat, aiTextureType_AMBIENT);
    material->diffuse   = GetAssimpMaterialColor(assimpMat, aiTextureType_DIFFUSE);
    material->specular  = GetAssimpMaterialColor(assimpMat, aiTextureType_SPECULAR);
    material->emission  = GetAssimpMaterialColor(assimpMat, aiTextureType_EMISSIVE);
    material->shininess = GetAssimpMaterialColor(assimpMat, aiTextureType_SHININESS).r;

    // Process material textures.
    material->ambientTexture  = GetAssimpMaterialTexture(assimpMat, aiTextureType_AMBIENT,   modelPath);
    material->diffuseTexture  = GetAssimpMaterialTexture(assimpMat, aiTextureType_DIFFUSE,   modelPath);
    material->specularTexture = GetAssimpMaterialTexture(assimpMat, aiTextureType_SPECULAR,  modelPath);
    material->emissionTexture = GetAssimpMaterialTexture(assimpMat, aiTextureType_EMISSIVE,  modelPath);
    material->shininessMap    = GetAssimpMaterialTexture(assimpMat, aiTextureType_SHININESS, modelPath);
    material->alphaMap        = GetAssimpMaterialTexture(assimpMat, aiTextureType_OPACITY,   modelPath);
    material->normalMap       = GetAssimpMaterialTexture(assimpMat, aiTextureType_NORMALS,   modelPath);

    // Finish sub-mesh loading.
    subMesh->LoadVertices(vertices, indices);
    subMesh->SetMaterial(material);
    subMesh->SetLoadingDone();
    return subMesh;
}

RGBA AssimpParser::GetAssimpMaterialColor(const aiMaterial* assimpMat , const aiTextureType& type)
{
    RGBA color;
    switch (type)
    {
    case aiTextureType_AMBIENT:
        assimpMat->Get(AI_MATKEY_COLOR_AMBIENT,  color.ptr(), NULL);
        break;
    case aiTextureType_DIFFUSE:
        assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE,  color.ptr(), NULL);
        break;
    case aiTextureType_SPECULAR:
        assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, color.ptr(), NULL);
        break;
    case aiTextureType_EMISSIVE:
        assimpMat->Get(AI_MATKEY_COLOR_EMISSIVE, color.ptr(), NULL);
        break;
    case aiTextureType_SHININESS:
        assimpMat->Get(AI_MATKEY_SHININESS,      color.ptr(), NULL);
        break;
    default:
        break;
    }
    return color;
}

Texture* AssimpParser::GetAssimpMaterialTexture(const aiMaterial* assimpMat, const aiTextureType& type, const std::string& modelPath)
{
    if (assimpMat->GetTextureCount(type) <= 0) return nullptr;
    
    aiString textureName;
    assimpMat->GetTexture(type, 0, &textureName);
    return resourceManager->Create<Texture>(modelPath + textureName.C_Str(), false);
}
