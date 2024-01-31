#pragma once
#include "Utils/PipelineData.h"
#include "Utils/Typedefs.h"
#include "Maths/Vertex.h"
#include <string>
#include <vector>

namespace Maths   { struct TangentVertex; }
namespace Scripts { struct ShaderScript;  }
namespace Anim    { class Skeleton;       }
namespace Resources
{
    class Material;
    class ShaderProgram;

    enum class SubMeshType
    {
        Static,
        Animated,
    };

    // - SubMesh parent - //
    class SubMesh
    {
    protected:
        std::string    name;
        SubMeshType    type;
        unsigned int   vertexCount       = 0;
        bool           loaded            = false;
        bool           sentToPipeline    = false;
        Material*      material          = nullptr;
        ShaderProgram* shaderPrograms[2] = { nullptr, nullptr }; // 1st: default shader, 2nd instanced shader.
        Utils::SubMeshPipelineData pipelineData;

    protected:
        SubMesh(std::string _name, const SubMeshType& _type) : name(std::move(_name)), type(_type) {}
        
    public:
        virtual ~SubMesh();/*
        SubMesh(const SubMesh&)            = delete;
        SubMesh(SubMesh&&)                 = delete;
        SubMesh& operator=(const SubMesh&) = delete;
        SubMesh& operator=(SubMesh&&)      = delete;*/
        
        // -- Getters -- //
        std::string                GetName()           const { return name;              } // Returns this sub-mesh's name.
        SubMeshType                GetType()           const { return type;              } // Returns this sub-mesh's type.
        uint                       GetVertexCount()    const { return vertexCount;       } // Returns this sub-mesh's vertex count.
        Utils::SubMeshPipelineData GetPipelineData()   const { return pipelineData;      } // Returns this sub-mesh's pipeline data.
        bool                       IsLoaded()          const { return loaded;            } // Returns True if this sub-mesh is loaded.
        bool                       WasSentToPipeline() const { return sentToPipeline;    } // Returns True if this sub-mesh was sent to the pipeline.
        Material*      GetMaterial()                   const { return material;          } // Returns this sub-mesh's material.
        ShaderProgram* GetShaderProgram()              const { return shaderPrograms[0]; } // Returns this sub-mesh's shader program.
        ShaderProgram* GetInstancedShaderProgram()     const { return shaderPrograms[1]; } // Returns this sub-mesh's instanced shader program.
        
        // -- Setters -- //
        void SetLoadingDone()          { loaded = true;         }
        void SetPipelineTransferDone() { sentToPipeline = true; }
        void SetPipelineData  (const Utils::SubMeshPipelineData& data);
        void SetMaterial              (Material*      _material     ) { material          = _material;      }
        void SetShaderProgram         (ShaderProgram* _shaderProgram) { shaderPrograms[0] = _shaderProgram; }
        void SetInstancedShaderProgram(ShaderProgram* _shaderProgram) { shaderPrograms[1] = _shaderProgram; }
        virtual void MakeInstanced(const uint& instanceBuffer) = 0;
    };

    // - Static SubMesh - //
    class StaticSubMesh : public SubMesh
    {
    protected:
        std::vector<Maths::TangentVertex> vertices;
        std::vector<uint>                 indices;
        
    public:
        // -- Constructor -- //

        // Creates a static sub-mesh with the given name.
        StaticSubMesh(const std::string& _name) : SubMesh(_name, SubMeshType::Static) {}
        ~StaticSubMesh() override                      = default;
        StaticSubMesh(const StaticSubMesh&)            = delete;
        StaticSubMesh(StaticSubMesh&&)                 = delete;
        StaticSubMesh& operator=(const StaticSubMesh&) = delete;
        StaticSubMesh& operator=(StaticSubMesh&&)      = delete;

        // -- Methods -- //
        
        void LoadVertices(const std::array<std::vector<float>, 3>& vertexData, std::array<std::vector<uint32_t>, 3> vertexIndices); // Loads vertices from arrays of vertex data and indices.
        void LoadVertices(const std::vector<Maths::TangentVertex>& vertexData, const std::vector<unsigned int>& vertexIndices);     // Loads vertices from arrays of vertex data and indices.
        
        virtual void SendVerticesToPipeline(size_t& totalVertexCount); // Sends the loaded vertices and indices to GPU memory.
        
        void MakeInstanced(const uint& instanceBuffer) override;

        // -- Getters -- //
        const std::vector<Maths::TangentVertex>& GetVertices() const { return vertices; } // Returns this sub-mesh's vertices.
        const std::vector<uint>&                 GetIndices () const { return indices;  } // Returns this sub-mesh's indices.
    };

    // - Animated SubMesh - //
    class AnimatedSubMesh : public SubMesh
    {
    private:
        std::vector<Maths::AnimatedVertex> vertices;
        std::vector<uint>                  indices;
        uint                               boneMatricesBuffer = 0;

    public:
        // -- Constructor -- //

        // Creates a sub-mesh with the given name.
        AnimatedSubMesh(const std::string& _name) : SubMesh(_name, SubMeshType::Animated) {}
        ~AnimatedSubMesh() override                        = default;
        AnimatedSubMesh(const AnimatedSubMesh&)            = delete;
        AnimatedSubMesh(AnimatedSubMesh&&)                 = delete;
        AnimatedSubMesh& operator=(const AnimatedSubMesh&) = delete;
        AnimatedSubMesh& operator=(AnimatedSubMesh&&)      = delete;

        // -- Methods -- //

        // Loads vertices from arrays of vertex data and indices.
        void LoadVertices(const std::vector<Maths::AnimatedVertex>& vertexData, const std::vector<uint>& vertexIndices);
        
        // Sends the loaded vertices and indices to GPU memory.
        void SendVerticesToPipeline(size_t& totalVertexCount);

        // Used by the mesh to give animated sub-meshes their skeleton's bone matrices buffer.
        void SetBoneMatricesBuffer(const uint& buffer) { if (!boneMatricesBuffer) boneMatricesBuffer = buffer; }

        void MakeInstanced(const uint& instanceBuffer) override;

        // -- Getters -- //
        const std::vector<Maths::AnimatedVertex>& GetVertices() const { return vertices; } // Returns this sub-mesh's vertices.
        const std::vector<uint>&                  GetIndices () const { return indices;  } // Returns this sub-mesh's indices.
        uint GetBoneMatricesBuffer() const { return boneMatricesBuffer; } // Returns this sub-mesh's bone matrices buffer ID.
    };
}