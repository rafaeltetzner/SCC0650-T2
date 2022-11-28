#pragma once

#include "util/types.h"
#include "event.h"

#include <glm/glm.hpp>

/**
 * @brief An abstraction od the camera class that provides input handling and matrices for use in opengl.
 * 
 */
class Camera
{
    public:
        Camera(){};

        /**
         * @brief Construct a new Camera
         * 
         * @param position The position at world space of the camera
         * @param up The world normal pointing upwards
         * @param yaw Horizontal angle of the camera view
         * @param pitch Vertical angle of the camera view
         * @param sensitivity How fast the camera moves for mouse inputs
         * @param speed How fast the camera moves for key inputs
         * @param zoom The camera fov, must be used to cotruct the perspective matrix
         */
        Camera(const glm::vec3& position,
               const glm::vec3& up, 
               f32 yaw=-90.0f, 
               f32 pitch=0.0f, 
               f32 sensitivity=0.1f, 
               f32 speed=10.0f,
               f32 zoom=45.0f);
        
        /**
         * @brief Get the view matrix of the camera
         * 
         * @return glm::mat4 The view matrix
         */
        glm::mat4 get_view() const;

        /**
         * @brief Get the fov of the camera
         * 
         * @return f32 The fov
         */
        f32 get_fov() const;

        /**
         * @brief Process key input, updating the position of the camera
         * 
         * @param key The pressed key
         * @param dt The game loop deltatime
         */
        void process_key_input(event::key::code key, f32 dt);

        /**
         * @brief Process cursor movement, internally calculates the offset for each function call
         * 
         * @param x The mouse position along the x axis
         * @param y The mouse position along the y axis
         */
        void process_cursor_movement(f32 x, f32 y);

        /**
         * @brief Process mouse scrolling, updates fov
         * 
         * @param offset The scroller offset
         */
        void process_scroll(f32 offset);

    private:
        glm::vec3 _position;
        glm::vec3 _up;
        glm::vec3 _front;
        glm::vec3 _right;
        glm::vec3 _world_up;

        f32 _yaw;
        f32 _pitch;

        f32 _speed;
        f32 _sensitivity;
        f32 _zoom;

        void update_vectors();
};