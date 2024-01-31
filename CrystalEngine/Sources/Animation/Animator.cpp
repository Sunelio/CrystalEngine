#include "Animation/Animator.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

#include "Debug/Logger.h"

#include <iostream>

using namespace Maths;
using namespace Anim;


Animator::~Animator()
{
    for (const auto& [name, anim] : animations)
        delete anim;
}

Animation* Animator::AddAnimation(const std::string& name)
{
    // If the animation doesn't already exist, create it.
    if (animations.find(name) == animations.end())
    {
        Animation* newAnim = new Animation(name, skeleton);
        animations.insert(std::pair(name, newAnim));
    }
    return animations[name];
}

Animation* Animator::GetAnimation(const std::string& name)
{
    if (animations.find(name) != animations.end())
        return animations[name];
    return nullptr;
}

bool Animator::IsAnimating() const
{
    return !curAnimName.empty();
}

Animation* Animator::GetCurrentAnimation()
{
    if (!curAnimName.empty())
        return animations[curAnimName];
    return nullptr;
}

std::string Animator::GetCurrentAnimationName()
{
   return curAnimName;
}

void Animator::SetCurrentAnimation(const std::string& name)
{
    if (animations.find(name) != animations.end() || name.empty())
    {
        if (IsAnimating()) GetCurrentAnimation()->Restart();
        curAnimName = name;
    }
    else
    {
        DebugLogWarning("Unable to set " + name + " as current animation because it isn't in this animator.");
    }
}

void Animator::Update(const float& deltaTime)
{
    // Update the current animation, then the skeleton's bones.
    if (IsAnimating()) GetCurrentAnimation()->Update(deltaTime);
    skeleton->UpdateBones();
}
