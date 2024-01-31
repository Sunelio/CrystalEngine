#version 450 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;
uniform vec2 offset;
uniform vec2 scale;

void main()
{
    gl_Position = vec4(inPos * scale + offset , 0.0, 1.0);
    texCoords   = inTexCoords;
}
