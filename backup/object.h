#pragma once

#include "model.h"

class Object
{
    public:
        Object(Model& model);
        void draw(Shader& shader);

        void rotate(glm::vec3 rotation_offset);
        void move(glm::vec3 offset);
        void scale(glm::vec3 scale_offset);

        void set_rotation(glm::vec3 rotation);
        void set_position(glm::vec3 position);
        void set_scale(glm::vec3 scale);

        glm::mat4 get_model_mat();
    private:
        Model& _model;
        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::vec3 _scale;
};