#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertColor;

out vec3 color;

void main()
{
   gl_Position = vec4(vertPos, 1.0);
   color = vertColor;
}
