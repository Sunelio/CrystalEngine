#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 fragPos;

uniform mat4 mvpMatrix;
uniform mat4 modelMat;

void main()
{
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
    fragPos     = (modelMat * vec4(aPos, 1.0)).xyz;
}