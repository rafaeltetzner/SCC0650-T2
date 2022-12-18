#include "application.h"
#include "util/logger.h"

#include "core/renderer.h"
#include <glm/gtc/matrix_transform.hpp>

void Application::init()
{
    _win.init("O P E N G L - 3 D", 1280, 720);

    _camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), _win.get_width(), _win.get_height());

    _win.set_key_callback([this](event::key::code code, event::key::action action)
    {
        switch(code)
        {
        case event::key::code::KEY_P:
            this->_camera.set_mode(Camera::mode::PERSPECTIVE);
            break;
        case event::key::code::KEY_O:
            this->_camera.set_mode(Camera::mode::ORTHO);
            break;
        default:
            this->_camera.process_key_movement(code, this->_delta_time);
            break;
        }
    });

    _win.set_mouse_callback([](event::mouse::button button, event::mouse::action action)
    {

    });
    _win.set_cursor_pos_callback([this](f64 x, f64 y)
    {
        this->_camera.process_cursor_movement(x, y);
    });
    _win.set_scroll_callback([this](f64 offset)
    {
        this->_camera.process_scroll(offset);
    });
    _win.show();
    _win.focus();
    _win.make_context();
}

void Application::run()
{
    Shader shader("res/shaders/vcode.vert", "res/shaders/fcode.frag");

    Model fox_model("res/fox/low-poly-fox-by-pixelmannen.obj");
    Model cube_model("res/cube/textured-cube.obj");
    Model shrek("res/shrek/shrek.obj");

    Light light;
    light.position = glm::vec3(0.0f);
    light.ambient = glm::vec3(0.5f);
    light.color = glm::vec3(1.0f);
    light.diffuse = glm::vec3(1.0f);
    light.specular = glm::vec3(1.0f);

    std::vector<renderer::Instance> instances;
    instances.push_back(shrek);
    instances.push_back(fox_model);
    instances.push_back(cube_model);

    instances[0].transform = glm::scale(glm::mat4(1.0f), glm::vec3(400.0f));

    f32 last_frame_time = 0.0f;
    f32 current_frame_time = 0.0f;

    while(!_win.should_close())
    {
        current_frame_time = glfwGetTime();
        _delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        _win.clear();
        
        shader.use();
        
        
        renderer::render(instances, shader, _camera, light);

        Window::poll_events();
        _win.swap_buffers();
    }
}

void Application::end()
{
}
