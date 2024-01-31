#pragma once
#include "Resource.h"
#include "SubMesh.h"
#include "Animation/Skeleton.h"
#include "Utils/Typedefs.h"
#include "../Externals/Includes/LightJson/JsonVirtual.h"
#include <string>
#include <vector>

namespace Anim { class Skeleton; }
namespace Resources
{
    // - Mesh (inherits from Resource) - //
    class Mesh : public Resource
    {
    protected:
        size_t          vertexCount       = 0;
        Anim::Skeleton* skeleton          = nullptr; // Non-null if the mesh is animated.
        uint            instanceMatBuffer = 0;       // Non-null if the mesh is instanced.
        
    public:
        // -- Attributes -- //
        std::vector<SubMesh*> subMeshes;

        // -- Constructor -- //
        
        // Creates a mesh with the given name.
        Mesh(const std::string& _name);
        ~Mesh() override;
        Mesh(const Mesh&)            = delete;
        Mesh(Mesh&&)                 = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&)      = delete;

        // -- Methods -- //
        void Load()           override; // Loads the mesh if all its sub-meshes are loaded.
        void SendToPipeline() override; // Sends all sub-meshes to GPU memory, then sends mesh to GPU memory.
        
        bool AreSubMeshesLoaded()          const; // Returns true if all sub-meshes are loaded.
        bool WereSubMeshesSentToPipeline() const; // Returns true if all sub-meshes have been sent to GPU memory.
        
        void SetSkeleton(Anim::Skeleton* _skeleton) { if (!skeleton) skeleton = _skeleton; }
        void MakeInstanced();
        
        size_t          GetVertexCount      () const { return vertexCount;      }
        size_t          GetSubMeshCount     () const { return subMeshes.size(); }
        Anim::Animator* GetAnimator         () const { return skeleton ? &skeleton->GetAnimator() : nullptr; }
        Anim::Skeleton* GetSkeleton         () const { return skeleton;          }
        uint            GetInstanceMatBuffer() const { return instanceMatBuffer; }
        bool            InAnimated          () const { return skeleton;          }
        bool            IsInstanced         () const { return instanceMatBuffer; }

        static ResourceType GetResourceType() { return ResourceType::Mesh; }
    };
}
