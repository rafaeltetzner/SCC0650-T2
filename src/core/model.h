#pragma once

#include "mesh.h"
#include <string>
#include <vector>

class Model
{
    public:
        Model(){};
        Model(const std::string& obj);
        void draw(Shader& shader);
    private:
        std::vector<Mesh> _meshes;
};