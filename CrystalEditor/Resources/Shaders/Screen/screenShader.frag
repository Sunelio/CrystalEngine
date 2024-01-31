#version 450 core

in vec2  texCoords;
out vec4 fragColor;

uniform sampler2D uiTexture;

void main()
{    
    fragColor = vec4(texture(uiTexture, texCoords));
}  