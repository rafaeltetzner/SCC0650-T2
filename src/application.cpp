#include "application.h"
#include "core/renderer.h"
#include "core/skybox.h"

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
    Shader default_shader("res/shaders/vcode.vs", "res/shaders/fcode.fs");
    Shader skybox_shader("res/shaders/skybox.vs", "res/shaders/skybox.fs");

    std::vector<std::string> faces = 
    {
        "res/skybox2/right.png",
        "res/skybox2/left.png",
        "res/skybox2/top.png",
        "res/skybox2/bottom.png",
        "res/skybox2/front.png",
        "res/skybox2/back.png"
    };
    Skybox skybox(faces);

    Model fox_model("res/fox/low-poly-fox-by-pixelmannen.obj");
    Model cube_model("res/cube/textured-cube.obj");
    Model shrek_model("res/shrek/shrek.obj");

    Light light;
    light.position = glm::vec3(10.0f);
    light.ambient = glm::vec3(0.2f);
    light.color = glm::vec3(1.0f);
    light.diffuse = glm::vec3(1.0f);
    light.specular = glm::vec3(1.0f);
    light.constant = 1.0f;
    light.linear = 0.0f;
    light.quadratic = 0.0f;

    std::vector<renderer::Instance> instances;
    instances.push_back(shrek_model);
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
        
        skybox.draw(skybox_shader, _camera);
        renderer::render(instances, default_shader, _camera, light);

        Window::poll_events();
        _win.swap_buffers();
    }
}

void Application::end()
{
}
