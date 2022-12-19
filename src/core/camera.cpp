#include "camera.h"
#include "util/logger.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, u32 width, u32 height, mode mode, f32 yaw, f32 pitch, f32 sensitivity, f32 speed, f32 zoom)
{
    _width = width;
    _height = height;
    _position = position;
    _mode = mode;
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
    float xLimit = 100;
    float yLimit = 100;

    glm::vec3 newPos;

    if(_position[0] > xLimit){
        newPos[0] = xLimit;
    } else if(_position[0] < -xLimit){
        newPos[0] = -xLimit;
    } else {
        newPos[0] = _position[0];
    }

    if(_position[2] > yLimit){
        newPos[2] = yLimit;
    } else if(_position[2] < -yLimit){
        newPos[2] = -yLimit;
    } else {
        newPos[2] = _position[2];
    }

    newPos[1] = _position[1];
    //newPos[1] = 0.0;

    return glm::lookAt(newPos, newPos + _front, _up);
    //return glm::lookAt(_position, _position + _front, _up);
}

f32 Camera::get_fov() const
{
    return _zoom;
}

void Camera::process_key_movement(event::key::code key, f32 dt)
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

void Camera::process_cursor_movement(f32 x, f32 y)
{
    static f32 last_x = x;
    static f32 last_y = y;

    f32 xoffset = x - last_x;
    f32 yoffset = last_y - y;
    last_x = x;
    last_y = y;

    xoffset *= _sensitivity;
    yoffset *= _sensitivity;

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
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    new_front.y = sin(glm::radians(_pitch));
    new_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(new_front);

    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}

glm::mat4 Camera::get_projection()
{
    switch(_mode)
    {
        case mode::PERSPECTIVE:
            return glm::perspective(glm::radians(_zoom), (f32)_width / (f32) _height, 0.1f, 1000.0f);
        case mode::ORTHO:
            return glm::ortho(-(_width / 2.0f)/ _zoom, +(_width / 2.0f) / _zoom, -(_height / 2.0f) / _zoom, +(_height / 2.0f) / _zoom, -1000.0f, 1000.0f);
        default:
            return glm::perspective(glm::radians(_zoom), (f32)_width / (f32) _height, 0.1f, 1000.0f);
    }
}