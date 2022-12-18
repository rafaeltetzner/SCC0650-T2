#version 330 core
out vec4 fs_color;

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D tex_diffuse;
    sampler2D tex_specular;
};

struct PointLight
{
    vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D texture_diffuse1;
uniform Material material;
uniform PointLight point_light;
uniform vec3 camera_pos;

vec3 calculate_ambient(Material material)
{
	return material.ambient;
}

vec3 calculate_diffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

vec3 calculate_specular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}

void main()
{    
    vec3 ambient_light = calculate_ambient(material);

    vec3 diffuse_light = calculate_diffuse(material, vs_position, vs_normal, point_light.position);

    vec3 specular_light = calculate_specular(material, vs_position, vs_normal, point_light.position, camera_pos);

    fs_color = texture(texture_diffuse1, vs_texcoord) * (
        vec4(ambient_light, 1.0f) + vec4(diffuse_light, 1.0f) + vec4(specular_light, 1.0f));
}