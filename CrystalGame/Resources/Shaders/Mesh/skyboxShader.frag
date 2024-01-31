#version 450 core

in  vec3 aTexCoord;
out vec4 fragColor;

uniform samplerCube modelCube;

void main()
{    
    fragColor = texture(modelCube, aTexCoord);
}
