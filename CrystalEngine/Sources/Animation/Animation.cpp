#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

using namespace Maths;
using namespace Anim;

Animation::Animation(std::string animName, Skeleton* baseSkeleton)
    : skeleton(baseSkeleton),
      name(std::move(animName))
{
    boneKeyframes.resize(skeleton->GetBoneCount());
    animatedBoneTransforms.resize(skeleton->GetBoneCount());
}

void Animation::Update(const float& deltaTime)
{
    if (!paused)
    {
        animationTimer += deltaTime * speed;

        for (size_t i = 0; i < boneKeyframes.size(); i++)
        {
            if (boneKeyframes[i].empty()) continue;
            
            // Change keyframe when the keyframe timer is finished.
            if (animationTimer >= boneKeyframes[i][curKeyframes[i]].time)
            {
                prevKeyframes[i] = curKeyframes[i];

                // Go to the next keyframe when not reversed.
                if (!reversed) {
                    curKeyframes[i]++;
                    if (curKeyframes[i] >= boneKeyframes[i].size())
                        curKeyframes[i] = 0;
                }
                // Go to the previous keyframe when reversed.
                else {
                    if (curKeyframes[i] > 0)
                        curKeyframes[i]--;
                    else
                        curKeyframes[i] = boneKeyframes[i].size()-1;
                }
            }
        }

        if (animationTimer >= duration)
            animationTimer = 0;
    }

    UpdateAnimatedBoneTransforms();
}

void Animation::Restart()
{
    for (size_t& curKeyframe : curKeyframes)
        curKeyframe = 0;
    animationTimer  = 0;
}

float Animation::GetCompletion() const
{
    return animationTimer / duration;
}

void Animation::SetCompletion(const float& completion)
{
    // Extract current keyframe and keyframe timer from completion percent.
    animationTimer = completion * duration;
    for (size_t i = 0; i < boneKeyframes.size(); i++)
    {
        for (size_t j = 0; j < boneKeyframes[i].size(); j++)
        {
            if (boneKeyframes[i][j].time >= animationTimer)
            {
                curKeyframes[i] = j;
                if (!reversed)
                    prevKeyframes[i] = (j == 0 ? boneKeyframes[i].size()-1 : j-1);
                else
                    prevKeyframes[i] = (j+1) % boneKeyframes[i].size();
            }
        }
    }

    UpdateAnimatedBoneTransforms();
}

RawTransform Animation::GetAnimatedBoneTransform(const size_t& boneId) const
{
    if (boneId > animatedBoneTransforms.size()) return RawTransform();
    return animatedBoneTransforms[boneId];
}

void Animation::UpdateAnimatedBoneTransforms()
{
    // Update each bone's animated local transform.
    for (size_t i = 0; i < skeleton->GetBoneCount(); i++)
    {
        const Bone* bone = skeleton->GetBone(i);
        if (boneKeyframes[bone->GetId()].empty()) continue;
        const Keyframe&  prevPoseKeyframe = boneKeyframes[bone->GetId()][prevKeyframes[bone->GetId()]];
        const Keyframe&  curPoseKeyframe  = boneKeyframes[bone->GetId()][curKeyframes [bone->GetId()]];
        const float      lerpVal      = remap(animationTimer, prevPoseKeyframe.time, curPoseKeyframe.time, 0, 1);
        const Vector3    animPosition = Vector3   ::Lerp (prevPoseKeyframe.transform.position, curPoseKeyframe.transform.position, lerpVal);
        const Quaternion animRotation = Quaternion::SLerp(prevPoseKeyframe.transform.rotation, curPoseKeyframe.transform.rotation, lerpVal);
        const Vector3    animScale    = Vector3   ::Lerp (prevPoseKeyframe.transform.scale,    curPoseKeyframe.transform.scale,    lerpVal);
        animatedBoneTransforms[bone->GetId()] = { animPosition, animRotation, animScale };
    }
}

void Animation::SetBoneKeyframes(const uint& boneID, const std::vector<Keyframe>& keyframes)
{
    boneKeyframes[boneID] = keyframes;
    curKeyframes .resize(boneKeyframes.size());
    prevKeyframes.resize(boneKeyframes.size());
}