#version 460 core
out vec4 color;
uniform vec3 editColorRGB;

in vec3 fragColor;
in vec2 fragTexCoords;

uniform sampler2D container;
uniform sampler2D elecfrog;

void main()
{

	// Texture Result
//	color = vec4(editColorRGB,1.0) * mix( texture(container, fragTexCoords),  texture(elecfrog, fragTexCoords), 0.2);
	
	// Exercise One
//	color = vec4(editColorRGB,1.0) * mix( texture(container, fragTexCoords),  texture(elecfrog, vec2(1.0f - fragTexCoords.x,fragTexCoords.y)), 0.2);

	// Exercise Two
	color = vec4(editColorRGB,1.0) * mix( texture(container, 2.0f*fragTexCoords),  texture(elecfrog, 2.0f*fragTexCoords), 0.2);

}