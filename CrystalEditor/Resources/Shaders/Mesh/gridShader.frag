#version 450

in vec3 fragPos;

out vec4 fragColor;

void main()
{
    float color = 0.4 - step(0.1, mod(fragPos.z, 5));
    if(color == 0) discard;
    
    fragColor = vec4(color, color, color, color);
}