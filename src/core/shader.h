#pragma once

#include "util/types.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

/**
 * @brief A simple wraper for a opengl program object
 * 
 */
class Shader
{
    public:
        Shader(){};

        /**
         * @brief Construct a new Shader object
         * 
         * @param vert_shader_filename Path to vertex shader file
         * @param frag_shader_filename Path to fragment shader file
         */
        Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename);

        /**
         * @brief Use this shader at current rendering state
         * 
         */
        void use() const;

        /**
         * @brief Allows to set uniforms like this:
         * 
         * ashader['color'] = glm::vec3(1.0,1.0,1.0);
         * 
         */
        class Uniform
        {
            public:
                Uniform() : _location(0) {};
                Uniform(u32 loc) : _location(loc) {};
                void set_f32(f32 value) const;
                void set_i32(i32 value) const;
                void set_vec2(const glm::vec2& value) const;
                void set_vec3(const glm::vec3& value) const;
                void set_vec4(const glm::vec4& value) const;
                void set_mat4(const glm::mat4& value) const;

                void operator=(f32 value) const;
                void operator=(i32 value) const;
                void operator=(const glm::vec2& value) const;
                void operator=(const glm::vec3& value) const;
                void operator=(const glm::vec4& value) const;
                void operator=(const glm::mat4& value) const;
            private:
                const u32 _location;
        };

        Uniform& operator[](const std::string& name);

    private:
        u32 _id;
        std::unordered_map<std::string, Uniform> _cache;
};