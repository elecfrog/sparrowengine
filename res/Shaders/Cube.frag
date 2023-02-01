#version 460 core
out vec4 color;

in vec3 Normals;
in vec2 texCoords;

// come from vertex shader
in vec3 ShadingPos;
uniform vec3 viewPos;

uniform float specular_intensity;


struct Light {
    vec3 k_ambient;
    vec3 k_diffuse;
    vec3 k_specular;
	
	vec3 position;
	vec3 direction;
	vec3 color;

	float p_constant;
	float p_linear;
	float p_quadratic;

	float s_outercutOffCos;
	float s_cutOffCos;
};
uniform Light _light;


struct Material {
    vec3 k_ambient;
    vec3 k_diffuse;
    vec3 k_specular;
    float shininess;

	sampler2D diffuse_map;
	
	sampler2D specular_map;

	sampler2D howlite;
}; 
uniform Material _material;


vec4 DirectionalLight()
{
	
	// ambient
	vec3 ambient = _material.k_ambient * _light.k_ambient;

	// diffuse
	vec3 norms = normalize(Normals);
	
	// If using Directional Light:
	vec3 lightDir = normalize(_light.direction);

	float diff = max(dot(norms, lightDir), 0.0f);
	vec3 diffuse = (_material.k_diffuse * diff) * texture(_material.diffuse_map, texCoords).rgb * _light.k_diffuse;

	// specular
	vec3 viewDir = normalize(viewPos - ShadingPos);
	vec3 reflectDir = reflect(lightDir, norms);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
	vec3 specular = (_material.k_specular * spec) * texture(_material.specular_map, texCoords).rgb * _light.k_specular;
	   
	vec3 res = (ambient + diffuse + specular);

	return vec4(res, 1.0f) ;
}

vec4 PointLight()
{
	float _distance = length(_light.position - ShadingPos);
	float attenuation = 1.0 /  (_light.p_constant  +  _light.p_linear  + _light.p_quadratic * (_distance * _distance));

	// ambient
	vec3 ambient = _material.k_ambient * _light.k_ambient*attenuation;

	// diffuse
	vec3 norms = normalize(Normals);
	vec3 lightDir = normalize(_light.position - ShadingPos);

	float diff = max(dot(norms, lightDir), 0.0f);
	vec3 diffuse = (_material.k_diffuse * diff) * texture(_material.diffuse_map, texCoords).rgb * _light.k_diffuse * attenuation;

	// specular
	vec3 viewDir = normalize(viewPos - ShadingPos);
	vec3 reflectDir = reflect(lightDir, norms);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
	vec3 specular = (_material.k_specular * spec) * texture(_material.specular_map, texCoords).rgb * _light.k_specular * attenuation;
	   

	vec3 res = (ambient + diffuse + specular);

	return vec4(res, 1.0f);
}

vec4 SpotLight()
{
	// ambient
	vec3 ambient = _material.k_ambient * _light.k_ambient;

	// diffuse
	vec3 norms = normalize(Normals);
	vec3 lightDir = normalize(_light.position - ShadingPos);
	float diff = max(dot(norms, lightDir), 0.0f);
	vec3 diffuse = (_material.k_diffuse * diff) * texture(_material.diffuse_map, texCoords).rgb * _light.k_diffuse;

	// specular
	vec3 viewDir = normalize(viewPos - ShadingPos);
	vec3 reflectDir = reflect(lightDir, norms);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
	vec3 specular = (_material.k_specular * spec) * texture(_material.specular_map, texCoords).rgb * _light.k_specular;
	   
	float theta = dot(lightDir, normalize(_light.direction));
	float epsilon = (_light.s_cutOffCos - _light.s_outercutOffCos);
	float intensity = clamp((theta - _light.s_outercutOffCos)/epsilon ,0.0f,1.0f);
	diffuse *= intensity;
	specular *= intensity;

	float _distance = length(_light.position - ShadingPos);
	float attenuation = 1.0 /  (_light.p_constant  +  _light.p_linear  + _light.p_quadratic * (_distance * _distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4((ambient + diffuse + specular), 1.0f);
}

vec4 DefaultLight()
{
	// ambient
	vec3 ambient = _material.k_ambient * _light.k_ambient;

	// diffuse
	vec3 norms = normalize(Normals);
	
	// If using Directional Light:
	vec3 lightDir = normalize(_light.position - ShadingPos);

	float diff = max(dot(norms, lightDir), 0.0f);
	vec3 diffuse = (_material.k_diffuse * diff) * texture(_material.diffuse_map, texCoords).rgb * _light.k_diffuse;

	// specular
	vec3 viewDir = normalize(viewPos - ShadingPos);
	vec3 reflectDir = reflect(lightDir, norms);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
	vec3 specular = (_material.k_specular * spec) * texture(_material.specular_map, texCoords).rgb * _light.k_specular;
	   

	vec3 res = (ambient + diffuse + specular);

	return vec4(res, 1.0f);
}

void main()
{	
	color = DefaultLight();
}
