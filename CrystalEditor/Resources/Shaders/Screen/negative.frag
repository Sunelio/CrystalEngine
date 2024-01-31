#version 450 core

in  vec2 texCoords;
out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
    fragColor = 1.0 - texture(screenTexture, texCoords) * 0.5;
}
