#include "window.h"
#include "../util/logger.h"

#include <GL/glew.h>

static u32 win_count = 0;

Window::~Window()
{
    if(!_handle)
        return;
    glfwDestroyWindow(_handle);
    win_count--;
    if(win_count == 0)
        glfwTerminate();
}

bool Window::init(const char* name, u32 width, u32 height)
{
    u32 res = glfwInit();
    if(!res)
    {
        logger::log::fatal("Failed to Initialize GLFW at window \"%s\" creation", name);
        return false;
    }
    logger::log::info("Initialized GLFW");

    _title = std::string(name);
    _handle = glfwCreateWindow(width, height, name, 0, 0);
    if(!_handle)
    {
        logger::log::error("Failed to generate window \"%s\"", name);
        return false;
    }

    glfwSetWindowUserPointer(_handle, &_callbacks);

    win_count++;

    return true;
}

bool Window::init(const std::string& name, u32 width, u32 height)
{
    return this->init(name.c_str(), width, height);
}

bool Window::make_context() const
{
    glfwMakeContextCurrent(_handle);
    if(glewInit() != GLEW_OK)
    {
        logger::log::fatal("Failed to initialize GLEW at context of window \"%s\"", _title.c_str());
        return false;
    }
    logger::log::trace("Made window \"%s\" current context", _title.c_str());
    return true;
}



void Window::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(_handle);
}

void Window::poll_events()
{
    glfwPollEvents();
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(_handle);
}

void Window::focus() const
{
    glfwFocusWindow(_handle);
}

void Window::show() const
{
    glfwShowWindow(_handle);
}

void Window::hide() const
{
    glfwHideWindow(_handle);
}

void Window::grab_cursor() const
{
    glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::release_cursor() const
{
    glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void Window::set_key_callback(event::key::callback_fn fn)
{
    _callbacks.key_callback = fn;
    glfwSetKeyCallback(_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        UserCallback& callbacks = *(UserCallback*) glfwGetWindowUserPointer(window);
        callbacks.key_callback
        (
            static_cast<event::key::code>(key), 
            static_cast<event::key::action>(action)
        );
    });
}

void Window::set_mouse_callback(event::mouse::callback_fn fn)
{
    _callbacks.mouse_callback = fn;
    glfwSetMouseButtonCallback(_handle, [](GLFWwindow* window, int button, int action, int mods)
    {
        UserCallback& callbacks = *(UserCallback*) glfwGetWindowUserPointer(window);
        callbacks.mouse_callback
        (
            static_cast<event::mouse::button>(button),
            static_cast<event::mouse::action>(action)
        );
    });
}

void Window::set_cursor_pos_callback(event::cursor_pos::callback_fn fn)
{
    _callbacks.cursor_pos_callback = fn;
    glfwSetCursorPosCallback(_handle, [](GLFWwindow* window, double xpos, double ypos)
    {
       UserCallback& callbacks = *(UserCallback*) glfwGetWindowUserPointer(window);
       callbacks.cursor_pos_callback(xpos, ypos);
    });
}

void Window::set_scroll_callback(event::scroll::callback_fn fn)
{
    _callbacks.scroll_callback = fn;
    glfwSetScrollCallback(_handle, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        UserCallback& callbacks = *(UserCallback*) glfwGetWindowUserPointer(window);
        callbacks.scroll_callback(yoffset);
    });
}