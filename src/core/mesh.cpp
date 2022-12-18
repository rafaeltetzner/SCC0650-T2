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
    shader["material_ambient"] = _material.ambient;
    shader["material_diffuse"] = _material.diffuse;
    shader["material_specular"] = _material.specular;

    glActiveTexture(GL_TEXTURE0 + 0);
    shader["material_tex_diffuse"].set_i32(0);
    glBindTexture(GL_TEXTURE_2D, _material.tex_diffuse.get_id());

    glActiveTexture(GL_TEXTURE0 + 1);
    shader["material_tex_specular"].set_i32(1);
    glBindTexture(GL_TEXTURE_2D, _material.tex_specular.get_id());

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}