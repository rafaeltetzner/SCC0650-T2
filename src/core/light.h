#pragma once

#include <glm/vec3.hpp>
#include "util/types.h"

struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};