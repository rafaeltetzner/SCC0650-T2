#include "application.h"
#include "util/logger.h"

#include "GL/glew.h"
#include "core/model.h"
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

//#include "mesh.h"

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
     _win.grab_cursor();
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


    /////////////////////////////////
    /*unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("tree.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);*/

    /////////////////////////////////

    Model person;
    person.load("res/FinalBaseMesh.obj", "textures/tree.jpg");

    Model person2;
    person2.load("res/FinalBaseMesh.obj", "textures/chairTexture.jpg");


    //Diz ao openGL para usar texturas com transparencia
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    
    GLuint buffer[2];
    glGenBuffers(2, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);


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
        person2.draw(shader);

        Window::poll_events();
        _win.swap_buffers();
    }
}
void Application::end()
{
}
