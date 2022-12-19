#include "application.h"
#include "core/renderer.h"
#include "core/skybox.h"

#include <glm/gtc/matrix_transform.hpp>

void Application::init()
{
    _win.init("O P E N G L - 3 D", 1280, 720);

    _camera = Camera(glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), _win.get_width(), _win.get_height());

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
    Model cube_model("res/plano/plano.obj");
    Model shrek_model("res/shrek/shrek.obj");
    Model campFire("res/CampFire/Fire.obj");
    Model cabana_model("res/cabana/cabana.obj");
    Model mesaRedonda("res/MesaRedonda/mesaRedonda.obj");
    Model pote("res/pote/poteCerto.obj");

    Light light;
    light.position = glm::vec3(0.0f, 10.0f, 0.0f);
    light.ambient = glm::vec3(0.2f);
    light.color = glm::vec3(1.0f);
    light.diffuse = glm::vec3(1.0f);
    light.specular = glm::vec3(1.0f);
    light.constant = 1.0f;
    light.linear = 0.0f;
    light.quadratic = 0.0f;

    std::vector<renderer::Instance> instances;
    instances.push_back(fox_model);
    instances.push_back(cube_model);
    instances.push_back(shrek_model);
    instances.push_back(campFire);
    instances.push_back(cabana_model);
    instances.push_back(mesaRedonda);
    instances.push_back(mesaRedonda);
    instances.push_back(pote);

    f32 meters = 1.0f;

    auto foxRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto foxScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.156f, 0.156f, 0.156f));
    auto foxTranslate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(-2.0894f, -97.249f, 0.32917f));
    instances[0].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * foxTranslate * foxScale * foxRotate;

    auto shrekRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto shrekScale = glm::scale(glm::mat4(1.0f), glm::vec3(3415.008f, 3415.008f, 3415.008f));
    auto shrekTranslate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(-1.104f, 72.618f, 8.6977f));
    instances[2].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * shrekTranslate * shrekScale * shrekRotate;

    auto campfireRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto campfireScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.585f, 0.585f, 0.585f));
    auto campfireTranslate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(-1.0f, -119.0f, 0.0f));
    instances[3].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * campfireTranslate * campfireScale * campfireRotate;

    auto cabanaRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto cabanaScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    auto cabanaTranslate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(0.0f, 0.0f, 0.0f));
    instances[4].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * cabanaTranslate * cabanaScale * cabanaRotate;

    auto mesa1Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto mesa1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(5.724f, 5.724f, 5.724f));
    auto mesa1Translate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(-12.0f, 17.0f, 6.0f));
    instances[5].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * mesa1Translate * mesa1Scale * mesa1Rotate;

    auto mesa2Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto mesa2Scale = glm::scale(glm::mat4(1.0f), glm::vec3(6.063f, 6.063f, 6.063f));
    auto mesa2Translate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(10.0f, 14.0f, 6.0f));
    instances[6].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * mesa2Translate * mesa2Scale * mesa2Rotate;
    

    auto poteRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto poteScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.042f, 0.042f, 0.042f));
    auto poteTranslate = glm::translate(glm::mat4(1.0f), meters * glm::vec3(-11.0f, 17.0f, 14.0f));
    instances[7].transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * poteTranslate * poteScale * poteRotate;
    
    
    f32 last_frame_time = 0.0f;
    f32 current_frame_time = 0.0f;

    f32 theta = 0.0f;

    while(!_win.should_close())
    {
        current_frame_time = glfwGetTime();
        _delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        _win.clear();
        
        skybox.draw(skybox_shader, _camera);

        instances[2].transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.104f, 72.618f, 8.6977f)) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(_delta_time*10.0f), glm::vec3(0.0f, 1.0f, 0.0f))* 
        glm::translate(glm::mat4(1.0f), glm::vec3(1.104f, -72.618f, -8.6977f)) * 
        instances[2].transform;

        light.position += 1.0f * glm::vec3(glm::sin(theta), 0.0f, glm::cos(theta));
        theta += 0.5 * _delta_time;
        if(theta >= 6.28f) theta = 0.0f;

        renderer::render(instances, default_shader, _camera, light);


        Window::poll_events();
        _win.swap_buffers();
    }
}

void Application::end()
{
}
