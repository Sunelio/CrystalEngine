#pragma once
#include "Bone.h"
#include "Animator.h"
#include <vector>

namespace Render    { class Renderer;     }
namespace Resources { class AssimpParser; }
namespace Anim
{
    // - Skeleton: Holds the bone hierarchy as well as an animator - //
    class Skeleton
    {
    private:
        Animator                 animator;
        Bone*                    rootBone;
        std::vector<Bone*>       bones;
        std::vector<Maths::Mat4> boneMatrices;
        uint                     boneMatricesBuffer = 0;
        Render::Renderer*        renderer;
        
        friend Render::Renderer;
        friend Resources::AssimpParser;
        
    public:
        Skeleton();
        ~Skeleton();
        Skeleton(const Skeleton&)            = delete;
        Skeleton(Skeleton&&)                 = delete;
        Skeleton& operator=(const Skeleton&) = delete;
        Skeleton& operator=(Skeleton&&)      = delete;
        
        void SendToPipeline();     // Sends the bone matrices to GPU memory.
        void UpdateBones(); // Updates the bone matrices according to the current animation.
        
        bool        DoesBoneExist(const Bone* bone)        const; // Returns true if the given bone exists in this skeleton.
        bool        DoesBoneExist(const uint& id)          const; // Returns true if a bone with the given ID exists in this skeleton.
        bool        DoesBoneExist(const std::string& name) const; // Returns true if a bone with the given name exists in this skeleton.
        const Bone* GetRootBone()                          const; // Returns the root of the bone hierarchy.
        const Bone* GetBone(const size_t& id)              const; // Returns the bone with the given ID, or nullptr if none is found.
        const Bone* GetBone(const std::string& name)       const; // Returns the bone with the given name, or nullptr if none is found.
        size_t      GetBoneCount() const          { return bones.size();       } // Returns the number of bones in this skeleton.
        uint        GetBoneMatricesBuffer() const { return boneMatricesBuffer; } // Returns the ID of the buffer that holds the bone matrices in GPU memory.
        Animator&   GetAnimator()                 { return animator;           } // Returns a reference to this skeleton's animator.

    private:
        void SetRootBone(Bone* bone);
        void AddBone    (Bone* bone);
        void RemoveBone (const int& id);
    };
}
