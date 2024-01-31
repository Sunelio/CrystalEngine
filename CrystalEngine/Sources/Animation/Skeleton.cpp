#include "Animation/Skeleton.h"
#include "Animation/Animation.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Maths/MathConstants.h"
#include <algorithm>

using namespace Maths;
using namespace Anim;

Skeleton::Skeleton() : animator(this), rootBone(nullptr), bones({})
{
	renderer = Core::Engine::Get()->GetRenderer();
}
Skeleton::~Skeleton()
{
	for (const Bone* bone : bones)
		delete bone;
	bones.clear();
	renderer->DeleteResource(this);
}

bool Skeleton::DoesBoneExist(const Bone* bone) const
{
	return std::find(bones.begin(), bones.end(), bone) != bones.end();
}

bool Skeleton::DoesBoneExist(const uint& id) const
{
	return std::find_if(bones.begin(), bones.end(), [id](const Bone* bone){ return bone->GetId() == id; }) != bones.end();
}

bool Skeleton::DoesBoneExist(const std::string& name) const
{
	return std::find_if(bones.begin(), bones.end(), [name](const Bone* bone){ return bone->GetName() == name; }) != bones.end();
}

const Bone* Skeleton::GetRootBone() const
{
	return rootBone;
}

const Bone* Skeleton::GetBone(const size_t& id) const
{
	const auto iterator = std::find_if(bones.begin(), bones.end(), [id](const Bone* bone){ return bone->GetId() == id; });
	if (iterator == bones.end()) return nullptr;
	return *iterator;
}

const Bone* Skeleton::GetBone(const std::string& name) const
{
	const auto iterator = std::find_if(bones.begin(), bones.end(), [name](const Bone* bone){ return bone->GetName() == name; });
	if (iterator == bones.end()) return nullptr;
	return *iterator;
}

void Skeleton::SendToPipeline()
{
	if (boneMatricesBuffer != 0) return;
	renderer->SendResourceToPipeline(this);
}

void Skeleton::UpdateBones()
{
	if (!rootBone) return;
	rootBone->RecursiveTransformUpdate();
	
	if (boneMatrices.size() != bones.size())
		boneMatrices.resize(bones.size());

	const bool isAnimating = animator.IsAnimating();
	for (size_t i = 0; i < bones.size(); i++)
	{
		if (isAnimating) boneMatrices[i] = bones[i]->GetOffsetMatrix() * bones[i]->GetAnimatedTransform().GetWorldMat() * Quaternion::FromPitch(-PI/2).ToMatrix(); // TODO: Fix rotated models on import.
		else             boneMatrices[i] = Mat4();
	}
	if (boneMatricesBuffer)
	{
		renderer->SetShaderStorageBufferData(boneMatricesBuffer, sizeof(Mat4) * boneMatrices.size(), boneMatrices.data());
	}
}

void Skeleton::SetRootBone(Bone* bone)
{
	if (rootBone) return;
	rootBone = bone;
}
void Skeleton::AddBone(Bone* bone)
{
	if (!DoesBoneExist(bone))
		bones.push_back(bone);
}

void Skeleton::RemoveBone(const int& id)
{
	const Bone* bone = GetBone(id);
	if (!DoesBoneExist(bone)) return;
	bones.erase(std::remove(bones.begin(), bones.end(), bone), bones.end());
	delete bone;
}
