#version 430 core
#define MAX_BONES          250
#define MAX_BONE_INFLUENCE 4

layout(location = 0) in vec3  aPos;
layout(location = 1) in vec2  aTexCoord;
layout(location = 2) in vec3  aNormal;
layout(location = 3) in vec3  aTangent;
layout(location = 4) in vec3  aBitangent;
layout(location = 5) in ivec4 aBoneIds;
layout(location = 6) in vec4  aBoneWeights;
layout(std430, binding = 7) buffer ssbo
{
    mat4 boneMatrices[MAX_BONES];
};

uniform mat4 mvpMatrix;
uniform mat4 modelMat;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 tbnMatrix;

void main()
{
    vec4 animatedPos       = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 animatedNormal    = vec3(0.0, 0.0, 0.0);
    vec3 animatedTangent   = vec3(0.0, 0.0, 0.0);
    vec3 animatedBitangent = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (aBoneIds[i] < 0 || aBoneIds[i] >= MAX_BONES) continue;
        animatedPos       += aBoneWeights[i] * (boneMatrices[aBoneIds[i]] * vec4(aPos, 1.0));
        animatedNormal    += aBoneWeights[i] * (mat3(boneMatrices[aBoneIds[i]]) * aNormal);
        animatedTangent   += aBoneWeights[i] * (mat3(boneMatrices[aBoneIds[i]]) * aTangent);
        animatedBitangent += aBoneWeights[i] * (mat3(boneMatrices[aBoneIds[i]]) * aBitangent);
    }

    gl_Position    = mvpMatrix * vec4(animatedPos.xyz, 1.0);
	fragPos        = (modelMat * animatedPos).xyz;
	fragTexCoord   = aTexCoord;
	fragNormal     = normalize(mat3(modelMat) * animatedNormal);
	vec3 tangent   = normalize(mat3(modelMat) * animatedTangent);
	vec3 bitangent = normalize(mat3(modelMat) * animatedBitangent);
	tbnMatrix      = mat3(tangent, bitangent, fragNormal);
}