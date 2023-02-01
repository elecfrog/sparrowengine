#version 460 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoords;

out vec3 fragColor;
out vec2 fragTexCoords;

uniform mat4 M; // Model Matrix
uniform mat4 V; // View  Matrix
uniform mat4 P; // Proj  Matrix

void main()
{
	gl_Position = P * V * M * vec4(aPos.x, aPos.y, aPos.z, 1.0);

//	fragColor = aColor;
	fragTexCoords = aTexCoords;
}