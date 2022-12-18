#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_position = vec4(model * vec4(aPos, 1.f)).xyz;
	vs_texcoord = aTexCoords;
	vs_normal = mat3(transpose(inverse(model))) * aNormal;


    gl_Position = projection * view * model * vec4(aPos, 1.0);
}