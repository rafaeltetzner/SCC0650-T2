#pragma once

#include "core/window.h"
#include "core/camera.h"

class Application
{
    public:
        Application(){};
        void init();
        void run();
        void end();
    private:
        Window _win;
        Camera _camera;
        f32 _delta_time;
        f32 _width;
        f32 _height;
};