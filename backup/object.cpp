#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(Model& model) : _model(model)
{
    _position = glm::vec3(0.0f);
    _scale    = glm::vec3(1.0f);
    _rotation = glm::vec3(0.0f);
}

void Object::draw(Shader& shader)
{
    shader["model"] = get_model_mat();
    _model.draw(shader);
}

void Object::rotate(glm::vec3 rotation_offset)
{
    _rotation += rotation_offset;
}

void Object::move(glm::vec3 offset)
{
    _position += offset;
}

void Object::scale(glm::vec3 scale_offset)
{
    _scale += scale_offset;
}

void Object::set_rotation(glm::vec3 rotation)
{
    _rotation = rotation;
}

void Object::set_position(glm::vec3 position)
{
    _position = position;
}

void Object::set_scale(glm::vec3 scale)
{
    _scale = scale;
}

glm::mat4 Object::get_model_mat()
{
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), _scale);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), _position);
    glm::mat4 rotation_x = glm::rotate(glm::mat4(1.0f), _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotation_y = glm::rotate(glm::mat4(1.0f), _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_z = glm::rotate(glm::mat4(1.0f), _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return translation * scale * rotation_x * rotation_y * rotation_z;
}