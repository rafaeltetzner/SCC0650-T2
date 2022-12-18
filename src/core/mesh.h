#pragma once

#include "shader.h"
#include "texture.h"
#include <glm/vec3.hpp>
#include <vector>

struct Vertex
{
    Vertex(){};
    Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : position(pos), normal(nor), texture_coords(tex) {};
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coords;
};

// struct Texture
// {
//     enum Type {DIFFUSE = 0, SPECULAR = 1, NORMAL = 2, HEIGHT = 3};
//     Texture(){};
//     Texture(u32 id, Type type) : id(id), type(type) {};
//     u32 id;
//     Type type;
// };

class Mesh
{
    public:
        struct Material
        {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            Texture tex_diffuse;
            Texture tex_specular;
        };
        Mesh() : _vao(0), _vbo(0) {};
        Mesh(const std::vector<Vertex>& vertices, const Material& material)
        {
            init(vertices, material);
        }

        void init(const std::vector<Vertex>& vertices, const Material& material);
        void draw(Shader& shader);
    private:
        u32 _vao, _vbo;
        std::vector<Vertex> _vertices;
        Material _material;
};