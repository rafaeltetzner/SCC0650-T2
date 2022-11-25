#include "shader.h"
#include "util/logger.h"

#include <GL/glew.h>

char* read_file(const char* filename)
{
    // Open file
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        logger::log::error("Failed to open file \"%s\" at %s:%s", filename, __FUNCTION__, __FILE__);
        return 0;
    }

    // Gets file total size
    fseek(file, 0, SEEK_END);
    u32 len = ftell(file);
    rewind(file);

    // Allocates memory for string containing whole file
    char* out = new char[len + 1];
    if(!out)
    {
        fclose(file);
        logger::log::error("Failed to allocate %d bytes of memory at %s:%s", len + 1, __FUNCTION__, __FILE__);
        return 0;
    }

    // Read File
    if(!fread(out, len, 1, file))
    {
        fclose(file);
        delete[] out;
        logger::log::error("Failed to read file \"%s\" at %s:%s", filename, __FUNCTION__, __FILE__);
        return 0;
    }

    logger::log::info("Read file \"%s\"", filename);
    fclose(file);
    return out;
}

Shader::Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename)
{
    // Read files
    char* vert_source = read_file(vert_shader_filename.c_str());
    char* frag_source = read_file(frag_shader_filename.c_str());

    // TODO: if shaders are invalid, use default shader

    // Create program
    _id = glCreateProgram();

    // Create shaders
    u32 vsh = glCreateShader(GL_VERTEX_SHADER);
    u32 fsh = glCreateShader(GL_FRAGMENT_SHADER);

    // Specify source
    glShaderSource(vsh, 1, &vert_source, 0);
    glShaderSource(fsh, 1, &frag_source, 0);

    char shader_log[1024] = {0};
    i32 is_compiled = 0;

    // Compile shaders
    glCompileShader(vsh);
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &is_compiled);
    if(!is_compiled)
    {
        glGetShaderInfoLog(vsh, sizeof(shader_log), 0, shader_log);
        logger::log::error("Failed to compile shader \"%s\" with: %s", vert_shader_filename.c_str(), shader_log);
    }
    else
        logger::log::info("Compiled shader \"%s\"", vert_shader_filename.c_str());
    
    glCompileShader(fsh);
    glGetShaderiv(fsh, GL_COMPILE_STATUS, &is_compiled);
    if(!is_compiled)
    {
        glGetShaderInfoLog(fsh, sizeof(shader_log), 0, shader_log);
        logger::log::error("Failed to compile shader \"%s\" with: %s", frag_shader_filename.c_str(), shader_log);
    }
    else
        logger::log::info("Compiled shader \"%s\"", frag_shader_filename.c_str());


    // Attach shaders to program
    glAttachShader(_id, vsh);
    glAttachShader(_id, fsh);


    // Link shaders
    i32 is_linked = 0;
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &is_linked);
    if(!is_linked)
    {
        glGetProgramInfoLog(_id, sizeof(shader_log), NULL, shader_log);
        logger::log::error("Failed to link program composed by \"%s\" and \"%s\" with: %s",
            frag_shader_filename.c_str(), vert_shader_filename.c_str(), shader_log);
    }

    logger::log::info("Created Shader composed by \"%s\" and \"%s\"",
        frag_shader_filename.c_str(), vert_shader_filename.c_str());


    // Clear memory
    glDeleteShader(vsh);
    glDeleteShader(fsh);
    delete vert_source;
    delete frag_source;
}

void Shader::use() const
{
    glUseProgram(_id);
}

Shader::Uniform& Shader::operator[](const std::string& name)
{
    if(_cache.find(name) == _cache.end())
    {
        u32 loc = glGetUniformLocation(_id, name.c_str());
        _cache.emplace(name, Uniform(loc));
        logger::log::trace("Adding uniform \"%s\" (%d) to shader (%d) cache, remember to use shader", name.c_str(), loc, _id);
    }
    return _cache[name];
}


void Shader::Uniform::operator=(f32 value) const
{
    glUniform1f(_location, value);
}

void Shader::Uniform::operator=(i32 value) const
{
    glUniform1i(_location, value);
}

void Shader::Uniform::operator=(const glm::vec2& value) const
{
    glUniform2f(_location, value.x, value.y);
}

void Shader::Uniform::operator=(const glm::vec3& value) const
{
    glUniform3f(_location, value.x, value.y, value.z);
}

void Shader::Uniform::operator=(const glm::vec4& value) const
{
    glUniform4f(_location, value.x, value.y, value.z, value.w);
}

void Shader::Uniform::operator=(const glm::mat4& value) const
{
    glUniformMatrix4fv(_location, 1, GL_FALSE, &value[0][0]);
}