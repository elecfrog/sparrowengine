#version 460 core

// Ouput Data, the only is color
out vec4 color;

// Input Data from Vertex Shader
in vec3 fragNormals;
in vec2 fragTexCoords;
in vec3 vert_worldPos;

// Lighting Variables
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// Textures
struct Material {
	sampler2D container;
	sampler2D elecfrog;
	sampler2D container_s;
	sampler2D container_e;
};
uniform Material rt;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec3 direction;
};
uniform Light lit;

vec3 emssion_item()
{
	return texture(rt.container_e, 2.0f*fragTexCoords).rgb;
}

vec3 basic_lighting()
{
	// material diffuse color
	vec3 MaterialDiffuseColor = mix( texture(rt.container, 2.0f*fragTexCoords).rgb,  texture(rt.elecfrog, 2.0f*fragTexCoords).rgb, 0.2);
	vec3 MaterialSpecularColor = texture(rt.container_s, 2.0f*fragTexCoords).rgb;
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = lit.ambient * lightColor;

	// diffuse lighting
	vec3 norm = normalize(fragNormals);
	vec3 lightDir = normalize(lightPos - vert_worldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lit.diffuse * diff * lightColor;

	// specular lighting
	vec3 viewDir = normalize(viewPos - vert_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = lit.specular * spec * lightColor;

	return (ambient * MaterialDiffuseColor + diffuse * MaterialDiffuseColor + specular * MaterialSpecularColor * 1.2f);
}

vec3 spot_lighting()
{
	// material diffuse color
	vec3 MaterialDiffuseColor = mix( texture(rt.container, 2.0f*fragTexCoords).rgb,  texture(rt.elecfrog, 2.0f*fragTexCoords).rgb, 0.2);
	vec3 MaterialSpecularColor = texture(rt.container_s, 2.0f*fragTexCoords).rgb;
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = lit.ambient * lightColor;

	// diffuse lighting
	vec3 norm = normalize(fragNormals);
	vec3 lightDir = normalize(lightPos - vert_worldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lit.diffuse * diff * lightColor;

	// specular lighting
	vec3 viewDir = normalize(viewPos - vert_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = lit.specular * spec * lightColor;

	return (ambient * MaterialDiffuseColor + diffuse * MaterialDiffuseColor + specular * MaterialSpecularColor * 1.2f);
}

vec3 point_lighting()
{
	// material diffuse color
	vec3 MaterialDiffuseColor = mix( texture(rt.container, 2.0f*fragTexCoords).rgb,  texture(rt.elecfrog, 2.0f*fragTexCoords).rgb, 0.2);
	vec3 MaterialSpecularColor = texture(rt.container_s, 2.0f*fragTexCoords).rgb;
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = lit.ambient * lightColor;

	// diffuse lighting
	vec3 norm = normalize(fragNormals);
	vec3 lightDir = normalize(lightPos - vert_worldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lit.diffuse * diff * lightColor;

	// specular lighting
	vec3 viewDir = normalize(viewPos - vert_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = lit.specular * spec * lightColor;

	float distance = length(lightPos - vert_worldPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));
	return attenuation * (ambient * MaterialDiffuseColor + diffuse * MaterialDiffuseColor + specular * MaterialSpecularColor * 1.2f);
}

vec3 directional_lighting()
{
	// material diffuse color
	vec3 MaterialDiffuseColor = mix( texture(rt.container, 2.0f*fragTexCoords).rgb,  texture(rt.elecfrog, 2.0f*fragTexCoords).rgb, 0.2);
	vec3 MaterialSpecularColor = texture(rt.container_s, 2.0f*fragTexCoords).rgb;
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = lit.ambient * lightColor;

	// diffuse lighting
	vec3 norm = normalize(fragNormals);
	vec3 lightDir = normalize(lit.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lit.diffuse * diff * lightColor;

	// specular lighting
	vec3 viewDir = normalize(viewPos - vert_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = lit.specular * spec * lightColor;

	return (ambient * MaterialDiffuseColor + diffuse * MaterialDiffuseColor + specular * MaterialSpecularColor * 1.2f);
}

void main()
{

	color = vec4(point_lighting(),1.0f);

}