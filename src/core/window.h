#pragma once

#include "event.h"
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>




class Window
{
    public:
    Window() : _handle(0), _title(""), _width(0), _height(0) {};
    ~Window();
    
    /**
     * @brief Creates a new window
     * 
     * @param name The name of the window (title).
     * @param width The width of the window
     * @param height The height of the window
     */
    bool init(const char* name, u32 width, u32 height);

    /**
     * @brief Creates a new window
     * 
     * @param name The name of the window (title).
     * @param width The width of the window
     * @param height The height of the window
     */
    bool init(const std::string& name, u32 width, u32 height);

    /**
     * @brief Makes the window the current context for opengl
     * 
     */
    bool make_context() const;

    /**
     * @brief Paint window white
     *
     */
    void clear() const;

    /**
     * @brief Swap buffers
     * 
     */
    void swap_buffers() const;

    /**
     * @brief Poll all window events
     * 
     */
    static void poll_events();

    /**
     * @brief Check whether or not the window should close.
     * 
     * @return true Window is open, may be hidden
     * @return false Window is closed, object must be destroyed
     */
    bool should_close() const;

    /**
     * @brief Focus the window
     * 
     */
    void focus() const;

    /**
     * @brief Show the window
     * 
     */
    void show() const;

    /**
     * @brief Hide the window
     * 
     */
    void hide() const;

    /**
     * @brief hides and grabs the cursor, providing virtual and unlimited cursor movement.
     *  This is useful for implementing for example 3D camera controls.
     * 
     */
    void grab_cursor() const;

    /**
     * @brief Makes the cursor visible and behaving normally.
     * 
     */
    void release_cursor() const;

    /**
     * @brief Set the key callback function, key stroke
     * 
     * @see event::key
     * 
     * @param fn The callback function
     */
    void set_key_callback(event::key::callback_fn fn);

    /**
     * @brief Set the mouse callback function, mouse click
     * 
     * @param fn The callback function
     */
    void set_mouse_callback(event::mouse::callback_fn fn);

    /**
     * @brief Set the cursor callback function, mouse move
     * 
     * @param fn The callback function
     */
    void set_cursor_pos_callback(event::cursor_pos::callback_fn fn);

    /**
     * @brief Set the scroll callback function, scroll up and down
     * 
     * @param fn The callback function
     */
    void set_scroll_callback(event::scroll::callback_fn fn);

    u32 get_width(){ return _width; }
    u32 get_height(){ return _height; }

    private:
    GLFWwindow* _handle;
    std::string _title;
    u32 _width;
    u32 _height;
    struct UserCallback {
        event::key::callback_fn key_callback;
        event::mouse::callback_fn mouse_callback;
        event::cursor_pos::callback_fn cursor_pos_callback;
        event::scroll::callback_fn scroll_callback;
    } _callbacks;
};
