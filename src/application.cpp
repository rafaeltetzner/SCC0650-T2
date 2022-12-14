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
    // _win.grab_cursor();
}
void Application::run()
{
    Shader shader("res/vcode.vert", "res/fcode.frag");
    
    // std::vector<Vertex> vertices = {
    //     Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0)),

    //     Vertex(glm::vec3(0.5f,  0.5f, 1.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(0.5f, -0.5f, 1.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0), glm::vec3(0)),
    //     Vertex(glm::vec3(-0.5f,  0.5f, 1.0f), glm::vec3(0), glm::vec3(0))
    // };
    // std::vector<Texture> textures = {
    //     Texture(0, Texture::Type::DIFFUSE)
    // };
    // std::vector<u32> indices = {
    //     0, 1, 3,
    //     1, 2, 3,

    //     0, 1, 4,
    //     1, 5, 4,

    //     0, 3, 4,
    //     7, 4, 0,

    //     3, 2, 6,
    //     6, 7, 3,

    //     1, 2, 6,
    //     6, 5, 1,

    //     5, 6, 7,
    //     7, 4, 5
    // };

    // Mesh mesh;
    // mesh.init(vertices, textures, indices);

    Model person;
    person.load("res/FinalBaseMesh.obj");

    f32 last_frame_time = 0.0f;
    f32 current_frame_time = 0.0f;

    while(!_win.should_close())
    {
        current_frame_time = glfwGetTime();
        _delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        _win.clear();

        // draw
        shader.use();
        
        glm::mat4 model         = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        glm::mat4 projection    = glm::perspective(glm::radians(_camera.get_fov()), (float)1280 / (float)720, 0.1f, 100.0f);
        glm::mat4 view          = _camera.get_view();

        shader["model"] = model;
        shader["view"] = view;
        shader["projection"] = projection;
        person.draw(shader);
        shader["model"] =  glm::rotate(glm::translate(model, glm::vec3(-15.0f, 0.0f, 0.0f)), glm::radians(90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        person.draw(shader);

        Window::poll_events();
        _win.swap_buffers();
    }
}
void Application::end()
{
}
