#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 tbnMatrix;

uniform mat4 mvpMatrix;
uniform mat4 modelMat;

void main()
{
	gl_Position    = mvpMatrix * vec4(aPos, 1.0);
	fragPos        = (modelMat * vec4(aPos, 1.0)).xyz;
	fragTexCoord   = aTexCoord;
	fragNormal     = normalize((modelMat * vec4(aNormal, 0.0))).xyz;
	vec3 tangent   = normalize(vec3(modelMat * vec4(aTangent,   0.0)));
	vec3 bitangent = normalize(vec3(modelMat * vec4(aBitangent, 0.0)));
	tbnMatrix      = mat3(tangent, bitangent, fragNormal);
}
