#version 450 core

in  vec2 texCoords;
out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
    vec2 texSize    = vec2(textureSize(screenTexture, 0).xy);
    vec2 fragSize   = vec2(1.0, 1.0) / texSize;
    vec4 horizontal = vec4(0.0, 0.0, 0.0, 0.0), vertical = vec4(0.0, 0.0, 0.0, 0.0);

    horizontal += texture(screenTexture, texCoords + vec2(0, fragSize.y)         ) *  1.0; // top left (factor +1)
    horizontal += texture(screenTexture, texCoords + vec2(fragSize.x, 0)         ) * -1.0; // bottom right (factor -1)
    vertical   += texture(screenTexture, texCoords                               ) * -1.0; // bottom left (factor -1)
    vertical   += texture(screenTexture, texCoords + vec2(fragSize.x, fragSize.y)) *  1.0; // top right (factor +1)

    float edge = sqrt(dot(horizontal, horizontal) + dot(vertical, vertical));
    fragColor = vec4(edge, edge, edge, edge);
}
