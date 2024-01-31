#version 450 core
#define PI 3.14159265358979323846

in  vec2 texCoords;
out vec4 fragColor;

uniform sampler2D screenTexture;
uniform float vignetteIntensity;

void main()
{
    fragColor = texture(screenTexture, texCoords);
    vec2 uv = texCoords;
    uv *=  1.0 - uv.yx;
    float vig = uv.x*uv.y * 15.0;
    vig = pow(vig, vignetteIntensity);
    fragColor *= vec4(vig, vig, vig, fragColor.a);
}
