#pragma once
#include "Maths/Color.h"
#include <string>
#include <unordered_map>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum   aiTextureType : int;

namespace Anim
{
	class Bone;
}
namespace Resources
{
	class Mesh;
	class AnimatedSubMesh;
	class Texture;
	class Material;
	class ResourceManager;

	// - AssimpParser: uses Assimp to parse model files (fbx) - //
	class AssimpParser
	{
	private:
		inline static ResourceManager* resourceManager = nullptr;
	
	public:
		static Mesh* ParseModelFile(const std::string& filename); // Parses the mesh in the given file.
	
	private:
		static std::unordered_map<std::string, aiNode*> CreateNodeMap(const aiNode* assimpNode);
		static AnimatedSubMesh* SubMeshFromAssimp(const aiScene* assimpScene, const aiMesh* assimpMesh, const int& meshIndex, const std::string& modelPath, std::unordered_map<std::string, Anim::Bone*>& bones);
		
		static Maths::RGBA GetAssimpMaterialColor  (const aiMaterial* assimpMat, const aiTextureType& type);
		static Texture*    GetAssimpMaterialTexture(const aiMaterial* assimpMat, const aiTextureType& type, const std::string& modelPath);
	};
}
