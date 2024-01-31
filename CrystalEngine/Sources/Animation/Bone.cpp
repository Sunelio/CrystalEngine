#include "Animation/Bone.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"

using namespace Maths;
using namespace Anim;

Bone::Bone(const uint& boneID, std::string boneName, Animator& skeletonAnimator)
	: id(boneID), name(std::move(boneName)), animator(skeletonAnimator) {}

void Bone::RecursiveTransformUpdate()
{
	if (!animator.IsAnimating()) return;
	
	// During an animation, use current animated bone transform.
	const auto [position, rotation, scale] = animator.GetCurrentAnimation()->GetAnimatedBoneTransform(id);
	animatedTransform.SetValues(position, rotation, scale);
	
	// Get the world matrix and update children.
	const Mat4 worldMat = animatedTransform.GetWorldMat();
	for (Bone* child : children)
	{
		child->animatedTransform.SetParentMat(worldMat);
		child->RecursiveTransformUpdate();
	}
}
