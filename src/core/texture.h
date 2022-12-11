#pragma once

#include "util/types.h"
#include <string>

class Texture
{
    public:
        enum type {NONE, DIFFUSE, SPECULAR};
        Texture() : _id(0), _width(0), _height(0), _type(type::NONE) {}
        Texture(std::string& filepath, type t);

    private:
        u32 _id;
        u32 _width;
        u32 _height;
        type _type;
};