#include "application.h"
#include "util/logger.h"

#include "core/renderer.h"

void Application::init()
{
    _win.init("O P E N G L - 3 D", 1280, 720);

    _camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), _win.get_width(), _win.get_height());

    _win.set_key_callback([this](event::key::code code, event::key::action action)
    {
        this->_camera.process_key_input(code, this->_delta_time);
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

    std::vector<renderer::Instance> instances;
    instances.push_back(fox_model);
    instances.push_back(cube_model);


    f32 last_frame_time = 0.0f;
    f32 current_frame_time = 0.0f;

    while(!_win.should_close())
    {
        current_frame_time = glfwGetTime();
        _delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        _win.clear();
        
        renderer::render(instances, shader, _camera);
        
        Window::poll_events();
        _win.swap_buffers();
    }
}

void Application::end()
{
}
