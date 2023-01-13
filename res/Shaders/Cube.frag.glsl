#version 460 core
out vec4 color;

in vec3 Normals;

void main()
{
   color = vec4(Normals, 1.0f);
}
