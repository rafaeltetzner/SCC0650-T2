#pragma once

#include "mesh.h"
#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
    public:
        Model() {};
        void load(const std::string& filepath);
        void draw(Shader& shader) const;
    private:
        std::vector<Mesh> _meshes;
        std::unordered_map<std::string, Texture> _tex_loaded;
        std::string _directory;

        void process_node(aiNode* node, const aiScene* scene);
        Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

        std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type);
        
};