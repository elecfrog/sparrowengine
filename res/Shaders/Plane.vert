#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 atexCoords;

out vec3 Normals;
out vec2 texCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
	texCoords = atexCoords;
}