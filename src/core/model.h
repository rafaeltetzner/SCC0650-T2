#pragma once

#include "mesh.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
    public:
        Model() {};
        void load(const std::string& filepath);
        void draw(const Shader& shader) const;
    private:
        std::vector<Mesh> _meshes;
        void process_node(aiNode* node, const aiScene* scene);
        Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
};