#shader vertex
#version 330 core

layout (location = 0) in vec3 position; // Positions/Coordinates
layout (location = 1) in vec3 aColors; // Colors
layout (location = 2) in vec2 aTexCoords; // Texture Coordinates

out vec3 v_Color; // Outputs the color for the Fragment Shader
out vec2 v_TexCoord; // Outputs the texture coordinates to the fragment shader

uniform float scale; // Controls the scale of the vertices
uniform mat4 u_MVP; // Inputs the matrices needed for 3D viewing with perspective

void main()
{

	gl_Position = u_MVP * vec4(position, 1.0f); 	// Outputs the positions/coordinates of all vertices

	v_Color = aColors; 	// Assigns the colors from the Vertex Data to "color"
	
	v_TexCoord = aTexCoords; 	// Assigns the texture coordinates from the Vertex Data to "texCoord"
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Color; // Inputs the color from the Vertex Shader
in vec2 v_TexCoord; // Inputs the texture coordinates from the Vertex Shader

// Gets the Texture Unit from the main function
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
//	color = vec4(v_Color,1.0f);
	color = texColor;

}