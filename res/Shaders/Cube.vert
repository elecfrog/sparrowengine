#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 atexCoords;

out vec3 Normals;
out vec2 texCoords;

out vec3 ShadingPos;

uniform mat4 u_VP;
uniform mat4 u_M;

void main()
{
	gl_Position = u_VP * u_M * vec4(aPos, 1.0);
	
	// pass FragPos to the fragment shader.

	ShadingPos = vec3(u_M * vec4(aPos, 1.0));
	
	// TODO: Why Using Normal Matrix?
	Normals = mat3(transpose(inverse(u_M))) * aNormals;
	texCoords = atexCoords;
}