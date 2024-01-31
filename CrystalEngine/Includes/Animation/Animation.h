#pragma once
#include "Maths/Transform.h"
#include "Utils/Typedefs.h"
#include <string>
#include <vector>

namespace Resources { class AssimpParser; }
namespace Anim
{
    class Skeleton;

    // - Keyframe: defines the position, rotation and scale of a bone at a certain time - //
    struct Keyframe
    {
        Maths::RawTransform transform;
        float time = 0;
    };

    // - Animation: holds transform keyframes for every bone and animates through them - //
    class Animation
    {
    private:
        // Stores the local transforms for each keyframe of each bone.
        // Rows: boneIDs
        // Columns: keyframes
        std::vector<std::vector<Keyframe>> boneKeyframes = {};

        // Stores smoothly animated local transform for each bone.
        std::vector<Maths::RawTransform> animatedBoneTransforms = {};
    
        std::vector<size_t> curKeyframes  = {};
        std::vector<size_t> prevKeyframes = {};
        float animationTimer = 0;
        
        Skeleton* skeleton;
        friend Resources::AssimpParser;
    
    public:
        const std::string name;
        float duration       = 0;
        float speed          = 1;
        bool  reversed       = false;
        bool  paused         = false;
    
        Animation(std::string animName, Skeleton* baseSkeleton);
        
        void Update(const float& deltaTime); // Updates the animation and bone transforms.
        void Restart(); // Restarts the animation from the beginning.

        float GetCompletion() const; // Returns a number that ranges from 0 at the start of the animation, to 1 at the end.
        void  SetCompletion(const float& completion); // Sets the animation's advancement to the given completion.

        Maths::RawTransform GetAnimatedBoneTransform(const size_t& boneId) const; // Returns the animated transform of the bone at the given ID.

    private:
        void UpdateAnimatedBoneTransforms();
        void  SetBoneKeyframes(const uint& boneID, const std::vector<Keyframe>& keyframes);
    };
}
