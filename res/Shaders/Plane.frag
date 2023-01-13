#version 460 core

in vec3 Normals;
in vec2 texCoords;

out vec4 color;
uniform vec3 u_color;
uniform sampler2D u_texture0;

void main()
{
   color = vec4(u_color, 1.0) * texture(u_texture0, texCoords);
}
