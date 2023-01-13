#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 v_UV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
	v_UV = aUV;
}