#pragma once
#include "Maths/Vertex.h"
#include <string>
#include <vector>

namespace Core { class ThreadManager; }
namespace Resources
{
    class ResourceManager;
    class ShaderProgram;
    class Resource;
    class Mesh;

    // - WavefrontParser: Custom OBJ and MTL loader - //
    class WavefrontParser
    {
    private:
        inline static Core::ThreadManager* threadManager   = nullptr;
        inline static ResourceManager*     resourceManager = nullptr;
        
    public:
        WavefrontParser()                                  = delete;
        ~WavefrontParser()                                 = delete;
        WavefrontParser(const WavefrontParser&)            = delete;
        WavefrontParser(WavefrontParser&&)                 = delete;
        WavefrontParser& operator=(const WavefrontParser&) = delete;
        WavefrontParser& operator=(WavefrontParser&&)      = delete;
        
        // -- Static Methods -- //
        static std::vector<Resource*> ParseMtl(const std::string& filename); // Loads materials and textures from the specified MTL file and returns them.
        static std::vector<Resource*> ParseObj(const std::string& filename); // Loads meshes, sub-meshes and materials from the specified OBJ file and returns them. 
    
    private:
        static void                 ParseMtlColor       (const std::string& line, float* colorValues);
        static void                 ParseObjVertexValues(const std::string& line, std::vector<float>& values, const int& startIndex, const int& valCount);
        static Maths::VertexIndices ParseObjIndices     (std::string indicesStr);
        static void                 ParseObjTriangle    (const std::string& line, std::array<std::vector<uint32_t>, 3>& indices);
        static std::array<std::vector<uint32_t>, 3> ParseObjSubMeshIndices(std::stringstream& fileContents);
        
        static void ParseObjObjectLine (const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources);
        static void ParseObjGroupLine  (const std::string& filename, const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources);
        static void ParseObjUsemtlLine (const std::string& filename, const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources);
        static void ParseObjIndicesLine(const std::string& filename, const std::string& line, std::stringstream& fileContents, Mesh*& mesh, const std::array<std::vector<float>, 3>& vertexData, std::vector<Resource*>& createdResources);
        
    };
}
