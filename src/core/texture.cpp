
#include "texture.h"
#include "util/logger.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(const std::string& path, Type type)
{
    _type = type;

    uchar* image = stbi_load(path.c_str(), &_width, &_height, &_n_channels, 0);

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(image)
    {
        GLenum format;
        switch(_n_channels)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA;break;
            default: break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        logger::log::trace("Loaded texture \"%s\"", path.c_str());
    }
    else
        logger::log::error("Failed to load texture \"%s\"", path.c_str());
    
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(image);
}

void Texture::bind(u32 unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind()
{
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}