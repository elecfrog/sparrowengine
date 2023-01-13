#version 460 core
out vec4 color;

in vec3 Normals;
in vec2 texCoords;

// come from vertex shader
in vec3 ShadingPos;
uniform vec3 viewPos;

uniform vec3 lightColor;
uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;
uniform int specular_intensity;

uniform vec3 lightPos;

uniform sampler2D texSlot_1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material _material;

void main()
{
	vec3 obj_color = vec3(1.0f);
	
	// ambient
	vec3 ambient = k_ambient * lightColor;

	// diffuse
	vec3 norms = normalize(Normals);
	vec3 lightDir = normalize(lightPos - ShadingPos);
	float diff = max(dot(norms, lightDir), 0.0f);
	vec3 diffuse = k_diffuse * diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - ShadingPos); // OK
	vec3 reflectDir = reflect(lightDir, norms);	// OK
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specular_intensity);
	vec3 specular = k_specular * spec * lightColor;
	   

	vec3 res = (ambient + diffuse + specular) * obj_color;

	color = vec4(res, 1.0f) * texture(texSlot_1, texCoords);
}
