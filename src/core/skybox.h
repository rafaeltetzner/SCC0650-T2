#pragma once

#include "model.h"
#include "camera.h"

class Skybox
{
    public:
        Skybox(const std::vector<std::string>& faces);
        void draw(Shader& shader, Camera& camera);
    private:
        u32 _vao, _vbo, _tex_id;
};