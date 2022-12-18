#include "application.h"
#include "util/logger.h"

#include "GL/glew.h"
#include "core/model.h"
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Application::init()
{
    _win.init("O P E N G L - 3 D", 1280, 720);

    _camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    _win.set_key_callback([this](event::key::code code, event::key::action action)
    {
        this->_camera.process_key_movement(code, this->_delta_time);
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
    // _win.grab_cursor();
}
void Application::run()
{
    Shader shader("res/shaders/vcode.vert", "res/shaders/fcode.frag");

    Model fox;
    fox.load("res/fox/low-poly-fox-by-pixelmannen.obj");

    Model box;
    box.load("res/cube/textured-cube.obj");

    f32 last_frame_time = 0.0f;
    f32 current_frame_time = 0.0f;

    glEnable(GL_DEPTH_TEST);

    while(!_win.should_close())
    {
        current_frame_time = glfwGetTime();
        _delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _win.clear();

        // draw
        shader.use();
        
        glm::mat4 model         = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        glm::mat4 projection    = glm::perspective(glm::radians(_camera.get_fov()), (float)1280 / (float)720, 0.1f, 100.0f);
        glm::mat4 view          = _camera.get_view();

        shader["model"] = model;
        shader["view"] = view;
        shader["projection"] = projection;
        fox.draw(shader);

        model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
        shader["model"] = model;
        box.draw(shader);

        Window::poll_events();
        _win.swap_buffers();
    }
}
void Application::end()
{
}
