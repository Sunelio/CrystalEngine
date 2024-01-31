#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Maths/Maths.h"
#include "Resources/SubMesh.h"
#include "Resources/ResourceManager.h"
#include "Scripts/ShaderScript.h"
#include <array>
#include <filesystem>

using namespace Maths;
using namespace Resources;
using namespace Render;
using namespace Utils;

SubMesh::~SubMesh()
{
    if (WasSentToPipeline())
        Core::Engine::Get()->GetRenderer()->DeleteResource<SubMesh>(this);
}

void SubMesh::SetPipelineData(const SubMeshPipelineData& data)
{
    if (!pipelineData.IsEmpty()) return;
    pipelineData = data;
}

void StaticSubMesh::LoadVertices(const std::array<std::vector<float>, 3>& vertexData, std::array<std::vector<uint32_t>, 3> vertexIndices)
{
    // Get the first index.
    uint32_t startIndex = 0;
    if (!indices.empty())
        startIndex = indices[indices.size()-1] + 1;
    
    // Add all parsed data to the vertices array.
    for (uint32_t i = 0; i < vertexIndices[0].size(); i++)
    {
        // Get the current vertex's position.
        Vector3 curPos = Vector3(vertexData[0][vertexIndices[0][i] * (size_t)3], vertexData[0][vertexIndices[0][i] * 3+1], vertexData[0][vertexIndices[0][i] * 3+2]);

        // Get the current vertex's texture coordinates.
        Vector2 curUv;
        if (!vertexData[1].empty()) curUv = Vector2(vertexData[1][vertexIndices[1][i] * (size_t)2], vertexData[1][vertexIndices[1][i] * 2+1]);

        // Get the current vertex's normal.
        Vector3 curNormal;
        if (!vertexData[2].empty()) curNormal = Vector3(vertexData[2][vertexIndices[2][i] * (size_t)3], vertexData[2][vertexIndices[2][i] * 3+1], vertexData[2][vertexIndices[2][i] * 3+2]);

        // Get the current face's tangent and bitangent.
        static Vector3 curTangent, curBitangent;
        if (i % 3 == 0)
        {
            const Vector3 edge1    = Vector3(vertexData[0][vertexIndices[0][i+1] * (size_t)3], vertexData[0][vertexIndices[0][i+1] * 3+1], vertexData[0][vertexIndices[0][i+1] * 3+2]) - curPos;
            const Vector3 edge2    = Vector3(vertexData[0][vertexIndices[0][i+2] * (size_t)3], vertexData[0][vertexIndices[0][i+2] * 3+1], vertexData[0][vertexIndices[0][i+2] * 3+2]) - curPos;
            const Vector2 deltaUv1 = Vector2(vertexData[1][vertexIndices[1][i+1] * (size_t)2], vertexData[1][vertexIndices[1][i+1] * 2+1]) - curUv;
            const Vector2 deltaUv2 = Vector2(vertexData[1][vertexIndices[1][i+2] * (size_t)2], vertexData[1][vertexIndices[1][i+2] * 2+1]) - curUv;

            const float f = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv2.x * deltaUv1.y);

            curTangent.x = f * (deltaUv2.y * edge1.x - deltaUv1.y * edge2.x);
            curTangent.y = f * (deltaUv2.y * edge1.y - deltaUv1.y * edge2.y);
            curTangent.z = f * (deltaUv2.y * edge1.z - deltaUv1.y * edge2.z);

            curBitangent.x = f * (-deltaUv2.x * edge1.x + deltaUv1.x * edge2.x);
            curBitangent.y = f * (-deltaUv2.x * edge1.y + deltaUv1.x * edge2.y);
            curBitangent.z = f * (-deltaUv2.x * edge1.z + deltaUv1.x * edge2.z);
        }

        // Create a new vertex with the computed data.
        vertices.push_back(TangentVertex{ curPos, curUv, curNormal, curTangent, curBitangent });
        indices .push_back(startIndex + i);
    }

    // TODO: Use the following code to prevent vertex dupplication for systems with low GPU memory.
    /*
    // Add all parsed data to the vertices array.
    std::unordered_map<TangentVertex, uint32_t> uniqueVertices{};
    for (uint32_t i = 0; i < vertexIndices[0].size(); i++)
    {
        // Same code as above for position, uv, normal, tangent, bitangent.

        // Create and save the current vertex.
        TangentVertex curVertex = { curPos, curUv, curNormal, curTangent, curBitangent };
        if (uniqueVertices.count(curVertex) <= 0) {
            uniqueVertices[curVertex] = startIndex + i;
            vertices.push_back(curVertex);
        }
        indices.push_back(startIndex + i);
    }
    */
}

void StaticSubMesh::LoadVertices(const std::vector<TangentVertex>& vertexData, const std::vector<unsigned>& vertexIndices)
{
    if (!vertices.empty() || !indices.empty()) return;
    vertices = vertexData;
    indices  = vertexIndices;
}

void AnimatedSubMesh::LoadVertices(const std::vector<AnimatedVertex>& vertexData, const std::vector<uint>& vertexIndices)
{
    vertices = vertexData;
    indices  = vertexIndices;
}

void StaticSubMesh::SendVerticesToPipeline(size_t& totalVertexCount)
{
    if (vertices.empty() || indices.empty() || !IsLoaded() || WasSentToPipeline())
        return;

    // Store the number of vertices in the model.
    vertexCount       = (unsigned int)indices.size();
    totalVertexCount += vertexCount;

    // Send vertices and indices through the pipeline.
    const Core::Engine* engine = Core::Engine::Get();
    engine->GetRenderer()->SendResourceToPipeline<StaticSubMesh>(this);
    if (!shaderPrograms[0]) shaderPrograms[0] = engine->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Mesh/meshShader.prog");
    if (!shaderPrograms[1]) shaderPrograms[1] = engine->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Mesh/instancedMeshShader.prog");

    // Free vertex arrays.
    // vertices.clear();
    // indices.clear();
    SetPipelineTransferDone();
}

void StaticSubMesh::MakeInstanced(const uint& instanceBuffer)
{
    if (pipelineData.IsInstanced()) return;
    pipelineData.instanceBO = instanceBuffer;
    Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<StaticSubMesh>(this);
}

void AnimatedSubMesh::SendVerticesToPipeline(size_t& totalVertexCount)
{
    if (vertices.empty() || indices.empty() || !IsLoaded() || WasSentToPipeline())
        return;

    // Store the number of vertices in the model.
    vertexCount       = (unsigned int)indices.size();
    totalVertexCount += vertexCount;

    // Send vertices and indices through the pipeline.
    const Core::Engine* engine = Core::Engine::Get();
    engine->GetRenderer()->SendResourceToPipeline<AnimatedSubMesh>(this);
    if (!shaderPrograms[0]) shaderPrograms[0] = engine->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Mesh/animShader.prog");
    if (!shaderPrograms[1]) shaderPrograms[1] = engine->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Mesh/instancedAnimShader.prog");

    // Free vertex arrays.
    // vertices.clear();
    // indices.clear();
    SetPipelineTransferDone();
}

void AnimatedSubMesh::MakeInstanced(const uint& instanceBuffer)
{
    if (!WasSentToPipeline() || pipelineData.IsInstanced()) return;
    pipelineData.instanceBO = instanceBuffer;
    Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<AnimatedSubMesh>(this);
}
