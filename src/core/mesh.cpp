#include "mesh.h"
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void Mesh::init(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<u32>& indices, const std::string& filepathTexture)
{
    _vertices = vertices;
    _textures = textures;
    _indices = indices;

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

    SDL_Surface *minha_imagem;

    minha_imagem = IMG_Load(filepathTexture.c_str());
    if(minha_imagem == NULL){
        throw("Unable to get a texture from the file");
    }

    glGenTextures(1, &(this->id_text));
    glBindTexture(GL_TEXTURE_2D, this->id_text);

    glBindTexture(GL_TEXTURE_2D, this->id_text); //Binda a textura na gpu
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Filtros de repeticao: GL_REPEAT -> repeticao simples
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtros de maginificao e minificacao: GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,minha_imagem->w,minha_imagem->h,0,GL_RGB,GL_UNSIGNED_BYTE,minha_imagem->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindVertexArray(0);
}

void Mesh::draw(const Shader& shader) const
{
    glBindVertexArray(_vao);
    glBindTexture(GL_TEXTURE_2D, this->id_text);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}