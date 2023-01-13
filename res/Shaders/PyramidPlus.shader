#shader vertex
#version 330 core

layout (location = 0) in vec3 position; // Positions/Coordinates
layout (location = 1) in vec3 aColors; // Colors
layout (location = 2) in vec2 aTexCoords; // Texture Coordinates
layout (location = 3) in vec3 aNormals; // Texture Coordinates


out vec3 v_Color; // Outputs the color for the Fragment Shader
out vec2 v_TexCoord; // Outputs the texture coordinates to the fragment shader
out vec3 v_Normals;
out vec3 v_currPos;

uniform float scale; // Controls the scale of the vertices
uniform mat4 u_MVP; // Inputs the matrices needed for 3D viewing with perspective
uniform mat4 u_Model; // Inputs the matrices needed for 3D viewing with perspective

void main()
{

	gl_Position = u_MVP * vec4(position, 1.0f); 	// Outputs the positions/coordinates of all vertices

	v_currPos = vec3(u_Model * vec4(position, 1.0f)); 

	v_Color = aColors; 	// Assigns the colors from the Vertex Data to "color"
	
	v_TexCoord = aTexCoords; 	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	v_Normals = aNormals;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Color; // Inputs the color from the Vertex Shader
in vec2 v_TexCoord; // Inputs the texture coordinates from the Vertex Shader
in vec3 v_Normals;
in vec3 v_currPos;

// Gets the Texture Unit from the main function
uniform vec4 u_Color;
uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform vec4 u_lightColor;

uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;

vec4 PointLight()
{
	vec3 lightVec = u_lightPos - v_currPos;

	float dist = length(lightVec);
	float a = 3.0f;
	float b = 0.7f;
	float inten = 1.0f / ( a * dist * dist +b * dist +1.0f);

	// ambient lighting
	float m_ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(v_Normals);
	vec3 lightDirection = normalize(lightVec);
	float m_diffuse = max(dot(normal, lightDirection), 0.0f) * inten;


	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(u_cameraPos - v_currPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float m_specular = specAmount * specularLight * inten;


	vec4 texColor = texture(u_Texture0, v_TexCoord);
//	color = vec4(v_Color,1.0f);
	return u_lightColor * texColor * ( m_ambient + m_diffuse) + u_lightColor * texture(u_Texture1, v_TexCoord) * m_specular;
}

vec4 DirectionalLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(v_Normals);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(u_cameraPos - v_currPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return ( texture(u_Texture0, v_TexCoord) * (diffuse + ambient) + texture(u_Texture1, v_TexCoord) * specular) * u_lightColor;
}

vec4 SpotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(v_Normals);
	vec3 lightDirection = normalize(u_lightPos - v_currPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(u_cameraPos - v_currPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	
	return ( texture(u_Texture0, v_TexCoord) * (diffuse * inten + ambient) + texture(u_Texture1, v_TexCoord) * specular * inten) * u_lightColor;

}


void main()
{
	color = PointLight();
}