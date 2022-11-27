#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, f32 yaw, f32 pitch, f32 sensitivity, f32 speed, f32 zoom)
{
    _position = position;
    _front = glm::vec3(0.0f, 0.0f, -1.0f);  
    _world_up = up;
    _yaw = yaw;
    _pitch = pitch;
    _sensitivity = sensitivity;
    _speed = speed;
    _zoom = zoom;
    this->update_vectors();
}

glm::mat4 Camera::get_view() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

f32 Camera::get_fov() const
{
    return _zoom;
}

void Camera::process_key_input(event::key::code key, f32 dt)
{
    using namespace event::key;
    f32 velocity = _speed * dt;
    switch(key)
    {
        case KEY_W:
            _position += _front * velocity;
            break;
        case KEY_S:
            _position -= _front * velocity;
            break;
        case KEY_A:
            _position -= _right * velocity;
            break;
        case KEY_D:
            _position += _right * velocity;
            break;
        case KEY_X:
            _position += _up * velocity;
            break;
        case KEY_Z:
            _position -= _up * velocity;
            break;
        default:
            break;
    }
}

#include "util/logger.h"

void Camera::process_cursor_movement(f32 x, f32 y)
{
    static f32 last_x = x;
    static f32 last_y = y;

    f32 xoffset = x - last_x;
    f32 yoffset = last_y - y;
    last_x = x;
    last_y = y;

    f32 sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    if(_pitch > 89.0f)
        _pitch = 89.0f;
    else if(_pitch < -89.0f)
        _pitch = -89.0f;

    this->update_vectors();
}
void Camera::process_scroll(f32 offset)
{
    _zoom -= offset;
    if(_zoom < 1.0f)
        _zoom = 1.0f;
    else if (_zoom > 45.0f)
        _zoom = 45.0f;
}

void Camera::update_vectors()
{
    logger::log::debug("Yaw{%f} - Pitch{%f}", _yaw, _pitch);
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    new_front.y = sin(glm::radians(_pitch));
    new_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(new_front);

    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}