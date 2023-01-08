#shader vertex
#version 330 core
			
layout(location = 0) in vec4 position;

uniform mat4 u_MVP; // Inputs the matrices needed for 3D viewing with perspective

void main()
{
	gl_Position = u_MVP * position;
};


#shader fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform vec4 u_lightColor;

void main()
{
	color = u_lightColor;
};