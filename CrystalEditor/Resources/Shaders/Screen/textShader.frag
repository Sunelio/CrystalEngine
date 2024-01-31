#version 450 core

in  vec2 texCoords;
out vec4 fragColor;

uniform sampler2D charTexture;
uniform vec4 textColor;

void main()
{    
    fragColor = textColor * vec4(1.0, 1.0, 1.0, texture(charTexture, texCoords).r);
}  