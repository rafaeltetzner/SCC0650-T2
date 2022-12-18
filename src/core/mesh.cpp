#include "mesh.h"
#include "util/logger.h"
#include <GL/glew.h>

#include <stdio.h>

void Mesh::init(const std::vector<Vertex>& vertices, const Material& material)
{
    _vertices = vertices;
    _material = material;

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
    
    glBindVertexArray(0);
}

void Mesh::draw(Shader& shader)
{
    u32 count_diffuse = 1;
    u32 count_specular = 1;

    for(u32 i = 0; i < _material.textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string num;
        Texture::Type type = _material.textures[i].get_type();
        std::string uniform_name;

        switch(type)
        {
            case Texture::Type::DIFFUSE:
                uniform_name = "texture_diffuse" + std::to_string(count_diffuse++);
                break;
            case Texture::Type::SPECULAR:
                uniform_name = "texture_specular" + std::to_string(count_specular++);
                break;
            default:
                break;
        }
        shader[uniform_name].set_i32(i);
        glBindTexture(GL_TEXTURE_2D, _material.textures[i].get_id());
    }

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}