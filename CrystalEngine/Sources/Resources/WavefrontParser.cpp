#include "Resources/WavefrontParser.h"
#include "Core/Engine.h"
#include "Resources/ResourceManager.h"
#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Maths/Vertex.h"
#include "Debug/Logger.h"
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
namespace cr = std::chrono;
using namespace Core;
using namespace Resources;
using namespace Maths;

std::vector<Resource*> WavefrontParser::ParseMtl(const std::string& filename)
{
    if (!resourceManager) resourceManager = Engine::Get()->GetResourceManager();

    // Read file contents and extract them to the data string.
    std::stringstream fileContents;
    {
        std::fstream f(filename, std::ios_base::in | std::ios_base::app);
        fileContents << f.rdbuf();
        f.close();
    }
    std::string filepath = fs::path(filename).parent_path().string() + "/";
    std::vector<Resource*> createdResources = {};

    // Read file line by line to create material data.
    std::string line;
    Material* curMaterial = nullptr;
    while (std::getline(fileContents, line)) 
    {
        // Create new material.
        if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w')
        {
            curMaterial = resourceManager->Create<Material>(line.substr(7, line.size() - 7));
            createdResources.push_back(curMaterial);
            continue;
        }

        // Don't touch material values if it is not yet created.
        if (curMaterial == nullptr)
            continue;
        line += ' ';

        // Read color values.
        if (line[0] == 'K')
        {
            switch (line[1])
            {
            case 'a':
                ParseMtlColor(line, curMaterial->ambient.ptr());
                continue;
            case 'd':
                ParseMtlColor(line, curMaterial->diffuse.ptr());
                continue;
            case 's':
                ParseMtlColor(line, curMaterial->specular.ptr());
                continue;
            case 'e':
                ParseMtlColor(line, curMaterial->emission.ptr());
                continue;
            default:
                break;
            }
        }

        // Read shininess.
        if (line[0] == 'N' && line[1] == 's')
        {
            curMaterial->shininess = std::strtof(line.substr(3, line.size()-4).c_str(), nullptr);
            continue;
        }

        // Read transparency.
        if (line[0] == 'd')
        {
            curMaterial->transparency = std::strtof(line.substr(2, line.size()-3).c_str(), nullptr);
            continue;
        }
        if (line[0] == 'T' && line[1] == 'r')
        {
            curMaterial->transparency = 1 - std::strtof(line.substr(2, line.size()-3).c_str(), nullptr);
            continue;
        }

        // Read texture maps.
        if (line[0] == 'm' && line[1] == 'a' && line[2] == 'p')
        {
            if (line[4] == 'K')
            {
                std::string texName = line.substr(7, line.size()-8);
                std::string texPath = filepath + texName;
                resourceManager->LoadFile(texPath);
                switch (line[5])
                {
                    // Ambient texture.
                    case 'a':
                        curMaterial->ambientTexture = resourceManager->Get<Texture>(texPath, true);
                    continue;

                    // Diffuse texture.
                    case 'd':
                        curMaterial->diffuseTexture = resourceManager->Get<Texture>(texPath, true);
                    continue;

                    // Specular texture.
                    case 's':
                        curMaterial->specularTexture = resourceManager->Get<Texture>(texPath, true);
                    continue;

                    // Emission texture.
                    case 'e':
                        curMaterial->emissionTexture = resourceManager->Get<Texture>(texPath, true);
                    continue;

                default:
                    break;
                }
            }
            // Shininess map.
            if (line[4] == 'N' && line[5] == 's')
            {
                const std::string texName = line.substr(7, line.size()-8);
                std::string texPath = filepath + texName;
                resourceManager->LoadFile(texPath);
                curMaterial->shininessMap = resourceManager->Get<Texture>(texPath, true);
                continue;
            }
            // Alpha map.
            if (line[4] == 'd')
            {
                const std::string texName = line.substr(6, line.size()-7);
                std::string texPath = filepath + texName;
                resourceManager->LoadFile(texPath);
                curMaterial->alphaMap = resourceManager->Get<Texture>(texPath, true);
                continue;
            }
        }
        {
            // Normal map.
            size_t bumpIndex = line.find("bump ");
            if (bumpIndex == std::string::npos) bumpIndex = line.find("Bump ");
            if (bumpIndex != std::string::npos)
            {
                std::string texName = line.substr(bumpIndex+5, line.size()-(bumpIndex+5)-1);
                std::string texPath = filepath + texName;
                resourceManager->LoadFile(texPath);
                curMaterial->normalMap = resourceManager->Get<Texture>(texPath, true);
                continue;
            }
        }
    }
    
    return createdResources;
}

std::vector<Resource*> WavefrontParser::ParseObj(const std::string& filename)
{
    if (!resourceManager) resourceManager = Engine::Get()->GetResourceManager();

    // Start chrono.
    cr::steady_clock::time_point chronoStart = cr::high_resolution_clock::now();

    // Read file contents and extract them to the data string.
    std::stringstream fileContents;
    {
        std::fstream f(filename, std::ios_base::in | std::ios_base::app);
        fileContents << f.rdbuf();
        f.close();
    }
    std::string filepath = fs::path(filename).parent_path().string() + "/";
    std::vector<Resource*> createdResources = {};

    // Define dynamic arrays for positions, uvs, and normals.
    // Format: vertexData[0] = positions, vertexData[1] = uvs, vertexData[2] = normals.
    std::array<std::vector<float>, 3> vertexData;

    // Store a pointer to the mesh group that is currently being created.
    Mesh* mesh = nullptr;

    // Read file line by line to create vertex data.
    std::string line;
    while (std::getline(fileContents, line))
    {
        line += ' ';
        switch (line[0])
        {
        case 'v':
            switch (line[1])
            {
                // Parse vertex coords and normals.
                case ' ':
                    ParseObjVertexValues(line, vertexData[0], 2, 3);
                break;
                // Parse vertex UVs.
                case 't':
                    ParseObjVertexValues(line, vertexData[1], 3, 2);
                break;
                // Parse vertex normals.
                case 'n':
                    ParseObjVertexValues(line, vertexData[2], 3, 3);
                break;
        default:
            break;
            }
            break;

        case 'o':
            ParseObjObjectLine(line, mesh, createdResources);
            break;

        case 'g':
            ParseObjGroupLine(filename, line, mesh, createdResources);
            break;

        case 'm':
            // ParseMtl(filepath + line.substr(7, line.size() - 8));
            resourceManager->LoadFile(filepath + line.substr(7, line.size() - 8), false);
            break;

        case 'u':
            ParseObjUsemtlLine(filename, line, mesh, createdResources);
            break;

        case 'f':
            ParseObjIndicesLine(filename, line, fileContents, mesh, vertexData, createdResources);
            break;

        default:
            break;
        }
    }
    if (mesh != nullptr && !mesh->subMeshes.empty()) {
        mesh->subMeshes.back()->SetLoadingDone();
        mesh->SetLoadingDone();
    }
    else {
        DebugLogWarning("Mesh has no sub-meshes after being loaded from obj file: " + filename);
    }

    // End chrono.
    cr::steady_clock::time_point chronoEnd = cr::high_resolution_clock::now();
    cr::nanoseconds elapsed = cr::duration_cast<cr::nanoseconds>(chronoEnd - chronoStart);
    DebugLogInfo(std::string("Loading file ") + filename + std::string(" took ") + std::to_string(elapsed.count() * 1e-9) + " seconds.");

    return createdResources;
}

#pragma region Value Parsing
void WavefrontParser::ParseMtlColor(const std::string& line, float* colorValues)
{
    size_t start = 3;
    size_t end = line.find(' ', start);

    for (int i = 0; i < 3; i++)
    {
        // Find the current coordinate value.
        std::string val = line.substr(start, end - start);

        // Set the current color value.
        colorValues[i] = std::strtof(val.c_str(), nullptr);

        // Update the start and end indices.
        start = end + 1;
        end = line.find(' ', start);
    }
}

void WavefrontParser::ParseObjVertexValues(const std::string& line, std::vector<float>& values, const int& startIndex, const int& valCount)
{
    // Find the start and end of the first value.
    size_t start = startIndex;
    size_t end = line.find(' ', start);
    if (end == start) {
        start++;
        end = line.find(' ', start);
    }

    for (int i = 0; i < valCount; i++)
    {
        // Find the current coordinate value.
        std::string val = line.substr(start, end - start);

        // Set the current value.
        values.push_back(std::strtof(val.c_str(), nullptr));

        // Update the start and end indices.
        start = end + 1;
        end = line.find(' ', start);
    }
}

VertexIndices WavefrontParser::ParseObjIndices(std::string indicesStr)
{
    VertexIndices vertex = { 0, 0, 0 };
    size_t start = 0;
    size_t end = indicesStr.find('/', start);
    for (int i = 0; i < 3 && indicesStr[start] != '\0'; i++)
    {
        // Find the current coordinate value.
        std::string index = indicesStr.substr(start, end - start);

        // Set the current index and skip double slashes.
        if (start != end)
            *(&vertex.pos + i) = std::atoi(index.c_str()) - 1;

        // Update the start and end indices.
        start = end + 1;
        end = indicesStr.find('/', start);
    }
    return vertex;
}

void WavefrontParser::ParseObjTriangle(const std::string& line, std::array<std::vector<uint32_t>, 3>& indices)
{
    size_t start = 2;
    size_t end = line.find(' ', start);
    if (end == start) {
        start++;
        end = line.find(' ', start);
    }

    for (int i = 0; i < 3; i++)
    {
        // Find the current coordinate value.
        const std::string indicesStr = line.substr(start, end - start) + '/';

        // Set the current index.
        VertexIndices vertexIndices = ParseObjIndices(indicesStr);
        for (size_t j = 0; j < 3; j++)
            indices[j].push_back(*((&vertexIndices.pos) + j));

        // Update the start and end line of the coord val.
        start = end + 1;
        end = line.find(' ', start);
    }
}

std::array<std::vector<uint32_t>, 3> WavefrontParser::ParseObjSubMeshIndices(std::stringstream& fileContents)
{
    // Holds all vertex data as indices to the vertexData array.
    // Format: indices[0] = pos, indices[1] = uvs, indices[3] = normals.
    std::array<std::vector<uint32_t>, 3> vertexIndices;

    // Read file line by line to create vertex data.
    std::string line;
    while (std::getline(fileContents, line)) 
    {
        line += ' ';
        switch (line[0])
        {
            // Parse object indices.
            case 'f':
                ParseObjTriangle(line, vertexIndices);
            continue;
            // Skip comments, smooth lighting statements and empty lines.
            case '#':
            case 's':
            case ' ':
            case '\n':
            case '\0':
                continue;
            // Stop whenever another symbol is encountered.
            default:
                fileContents.seekg(fileContents.tellg() - (std::streamoff)line.size());
            break;
        }
        break;
    }

    return vertexIndices;
}

#pragma endregion 

#pragma region Line Parsing
void WavefrontParser::ParseObjObjectLine(const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources)
{
    // Let the previous mesh send its vertices to the pipeline.
    if (mesh!= nullptr && !mesh->subMeshes.empty()) {
        mesh->subMeshes.back()->SetLoadingDone();
        mesh->SetLoadingDone();
    }

    mesh = resourceManager->Create<Mesh>(line.substr(2, line.size()-3), false);
    createdResources.push_back(mesh);
}

void WavefrontParser::ParseObjGroupLine(const std::string& filename, const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources)
{
    if (line[2] == '\0')
        return;

    // Make sure a mesh was already created.
    if (mesh == nullptr) {
        mesh = resourceManager->Create<Mesh>("mesh_" + fs::path(filename).stem().string(), false);
        createdResources.push_back(mesh);
    }

    // Send all vertex data from the previous sub-mesh to the pipeline.
    if (!mesh->subMeshes.empty()) {
        mesh->subMeshes.back()->SetLoadingDone();
        mesh->SetLoadingDone();
    }

    // Create a sub-mesh and add it to the mesh.
    mesh->subMeshes.push_back(new StaticSubMesh(line.substr(2, line.size() - 3)));
}

void WavefrontParser::ParseObjUsemtlLine(const std::string& filename, const std::string& line, Mesh*& mesh, std::vector<Resource*>& createdResources)
{
    // Make sure a mesh was already created.
    if (mesh == nullptr) {
        mesh = resourceManager->Create<Mesh>("mesh_" + fs::path(filename).stem().string(), false);
        createdResources.push_back(mesh);
    }

    // Make sure a sub-mesh was already created.
    if (mesh->subMeshes.empty())
        mesh->subMeshes.push_back(new StaticSubMesh("submesh_" + line.substr(7, line.size() - 8)));

    // Make sure the current sub-mesh doesn't already have a material.
    else if (mesh->subMeshes.back()->GetMaterial() != nullptr) {
        mesh->subMeshes.back()->SetLoadingDone();
        mesh->subMeshes.push_back(new StaticSubMesh("submesh_" + line.substr(7, line.size() - 8)));
    }

    // Set the current sub-mesh's material.
    mesh->subMeshes.back()->SetMaterial(resourceManager->Get<Material>(line.substr(7, line.size() - 8)));
}

void WavefrontParser::ParseObjIndicesLine(const std::string& filename, const std::string& line, std::stringstream& fileContents, Mesh*& mesh, const std::array<std::vector<float>, 3>& vertexData, std::vector<Resource*>& createdResources)
{
    // Make sure a mesh was already created.
    if (mesh == nullptr) {
        mesh = resourceManager->Create<Mesh>("mesh_" + fs::path(filename).stem().string(), false);
        createdResources.push_back(mesh);
    }

    // Make sure a sub-mesh was already created.
    if (mesh->subMeshes.empty())
        mesh->subMeshes.push_back(new StaticSubMesh("submesh_" + fs::path(filename).stem().string()));

    // Let the current sub-mesh parse its vertices.
    fileContents.seekg(fileContents.tellg() - (std::streamoff)line.size());
    ((StaticSubMesh*)mesh->subMeshes.back())->LoadVertices(vertexData, ParseObjSubMeshIndices(fileContents));
}
#pragma endregion 
