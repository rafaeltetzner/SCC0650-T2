#pragma once

#include "util/types.h"
#include <string>


class Texture
{
    public:
        enum Type {DIFFUSE = 0, SPECULAR = 1, NORMAL = 2, HEIGHT = 3};
        Texture(){};
        Texture(Type type, const std::string& path) : _id(0), _type(type), _path(path) { this->load(); };
        bool load();

        u32 get_id() const { return _id; };
        Type get_type() const { return _type; };
    private:
        u32 _id;
        i32 _width;
        i32 _height;
        i32 _n_channels;
        Type _type;
        std::string _path;
};