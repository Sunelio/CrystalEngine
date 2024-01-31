#version 450 core
#define PI 3.14159265358979323846

in  vec2 texCoords;
out vec4 fragColor;

uniform sampler2D screenTexture;

vec4 RGBAtoHSVA(vec4 rgba)
{
    vec4 hsva;

    float minV = min(min(rgba.r, rgba.g), rgba.b);
    float maxV = max(max(rgba.r, rgba.g), rgba.b);
    float diff = maxV - minV;

    float r = rgba.r;
    float g = rgba.g;
    float b = rgba.b;

    // Set Value.
    hsva.b = maxV;

    // If max and min are the same, return.
    if (diff < 0.00001)
        return vec4(0, 0, hsva.b, rgba.a);

    // Set Saturation.
    if (maxV > 0)
        hsva.g = diff / maxV;
    else
        return vec4(0, 0, hsva.b, rgba.a);

    // Set Hue.
    if (r >= maxV)
        hsva.r = (g - b) / diff;
    else if (g >= maxV)
        hsva.r = 2.0f + (b - r) / diff;
    else if (b >= maxV)
        hsva.r = 4.0f + (r - g) / diff;

    // Keep Hue above 0.
    if (hsva.r < 0)
        hsva.r += 2 * PI;

    hsva.a = rgba.a;
    return hsva;
}

void main()
{
    fragColor = texture(screenTexture, texCoords);
    vec4 hsva = RGBAtoHSVA(fragColor);
    fragColor = vec4(hsva.b, hsva.b, hsva.b, fragColor.a);
}
