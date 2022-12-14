#pragma once

#include "shader.h"
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

struct Texture
{
    enum Type {DIFFUSE = 0, SPECULAR = 1};
    Texture(){};
    Texture(u32 id, Type type) : id(id), type(type) {};
    u32 id;
    Type type;
};

class Mesh
{
    public:
        Mesh() : _vao(0), _vbo(0), _ebo(0) {};
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<u32>& indices)
        {
            init(vertices, textures, indices);
        }

        void init(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<u32>& indices);
        void draw(const Shader& shader) const;
    private:
        u32 _vao, _vbo, _ebo;
        std::vector<Vertex> _vertices;
        std::vector<Texture> _textures;
        std::vector<u32> _indices;
};