#pragma once

#include "util/types.h"
#include <string>


/**
 * @brief A simple opengl Texture object wrapper
 * 
 */
class Texture
{
    public:
        /** @brief Type of the texture **/
        enum Type {DIFFUSE = 0, SPECULAR = 1};
        Texture(){};
        /**
         * @brief Construct a new Texture object
         * 
         * @param path Path to the texture image file
         * @param type The type of the texture
         */
        Texture(const std::string& path, Type type);

        /**
         * @brief Binds texture to given unit
         * 
         * @param unit 
         */
        void bind(u32 unit);

        /**
         * @brief Unbinds ALL textures
         * 
         */
        void unbind();

        Type get_type(){ return _type; };
        u32 get_id(){ return _id; };
        
    private:
        u32 _id;
        i32 _width;
        i32 _height;
        i32 _n_channels;
        Type _type;
        std::string _path;
};