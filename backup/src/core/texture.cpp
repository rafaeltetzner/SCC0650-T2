
#include "texture.h"
#include "util/logger.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool Texture::load()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &_id);
    logger::log::debug("Id - %d", _id);

    uchar* data = stbi_load(_path.c_str(), &_width, &_height, &_n_channels, 0);
    logger::log::debug("%d %d %d", _width, _height, _n_channels);
    if(!data)
    {
        logger::log::error("Texture \"%s\" failed to load", _path.c_str());
        stbi_image_free(data);
        return false;
    }
    GLenum format;
    switch(_n_channels)
    {
        case 1: format = GL_RED; logger::log::debug("RED"); break;
        case 3: format = GL_RGB; logger::log::debug("RGB"); break;
        case 4: format = GL_RGBA; logger::log::debug("RGBA"); break;
        default: break;
    }

    glBindTexture(GL_TEXTURE_2D, _id);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    logger::log::trace("Loaded texture \"%s\" successfully", _path.c_str());

    return true;
}