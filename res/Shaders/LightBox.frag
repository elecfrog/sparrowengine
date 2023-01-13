#version 460 core
out vec4 color;

in vec3 Normals;

uniform vec3 lightColor;


void main()
{
   color = vec4(lightColor, 1.0f);
}
