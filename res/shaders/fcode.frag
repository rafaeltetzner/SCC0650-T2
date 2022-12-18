#version 330 core
out vec4 fs_color;

in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

uniform sampler2D material_tex_diffuse;
uniform sampler2D material_tex_specular;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

uniform vec3 camera_pos;

void main()
{   
    vec3 norm = normalize(vs_normal);
    vec3 viewDir = normalize(camera_pos - vs_position);

    vec3 lightDir = normalize(light_position - vs_position);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    
    // float distance = length(light.position - fragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient = light_ambient * vec3(texture(material_tex_diffuse, vs_texcoord));
    vec3 diffuse = light_diffuse * diff * vec3(texture(material_tex_diffuse, vs_texcoord));
    vec3 specular = light_specular * spec * vec3(texture(material_tex_specular, vs_texcoord));

    //ambient *= attenuation;
    //diffuse *= attenuation;
    //specular *= attenuation;
    
    fs_color = vec4(ambient + specular + diffuse, 1.0f);
    //fs_color = texture(material_tex_diffuse, vs_texcoord);
}