#version 450

// Input pos, uv, normal and matrix.
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormal;
in mat3 tbnMatrix;

out vec4 fragColor;

uniform vec3 Diffuse;

void main()
{
	if(Diffuse == vec3(0.0, 0.0, 0.0))
	{
		fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		fragColor = vec4(Diffuse.x, Diffuse.y, Diffuse.z, 1.0f);
	}
	//FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}