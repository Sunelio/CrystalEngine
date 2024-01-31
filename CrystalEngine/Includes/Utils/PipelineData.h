#pragma once

#include "Utils/Typedefs.h"

namespace Utils 
{
    // - SubMeshPipelineData: Holds sub-mesh data specific to the pipeline API - //
    struct SubMeshPipelineData
    {
        uint VAO = 0, VBO = 0, EBO = 0, instanceBO = 0;
        bool IsEmpty() const
        {
            return !(VBO && EBO && VAO);
        }
        bool IsInstanced() const
        {
            return instanceBO;
        }
    };

    // - RenderTexturePipelineData: Holds render texture data specific to the pipeline API - //
    struct RenderTexturePipelineData
    {
        uint FBO = 0, RBO = 0, ID = 0;
        bool IsEmpty() const
        {
            return !(FBO && RBO && ID);
        }
    };

    // - ScreenRectPipelineData: Holds screen rectangle data specific to the pipeline API - //
    struct ScreenRectPipelineData
    {
        uint VAO = 0, VBO = 0;
        bool IsEmpty() const
        {
            return !(VAO && VBO);
        }
    };
}