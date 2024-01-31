#version 450 core

layout(location = 0 ) in vec3 aPos;
layout(location = 1 ) in vec2 aTexCoord;
layout(location = 2 ) in vec3 aNormal;
layout(location = 3 ) in vec3 aTangent;
layout(location = 4 ) in vec3 aBitangent;
layout(location = 10) in mat4 aInstanceMat;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormal;
out mat3 tbnMatrix;

uniform mat4 mvpMatrix;
uniform mat4 modelMat;

void main()
{
	gl_Position    = mvpMatrix * aInstanceMat * vec4(aPos, 1.0);
	fragPos        = (modelMat * aInstanceMat * vec4(aPos, 1.0)).xyz;
	fragTexCoord   = aTexCoord;
	fragNormal     = normalize(mat3(modelMat * aInstanceMat) * aNormal   );
	vec3 Tangent   = normalize(mat3(modelMat * aInstanceMat) * aTangent  );
	vec3 Bitangent = normalize(mat3(modelMat * aInstanceMat) * aBitangent);
	tbnMatrix      = mat3(Tangent, Bitangent, fragNormal);
}
