#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Slice.h"
#include "Shader.h"

class Window {
private:
    GLFWwindow* window;
    Camera camera;
    Slice slice;
    Shader shader;
    double last_xpos, last_ypos, rate;
    CONSTANT::MODE m_mode;

    static void error_callback(int error, const char* description);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void set_callback();

public:
    Window();
    ~Window();

    Window(Window const& rhs) = delete;
    Window(Window&& rhs) = delete;
    Window& operator=(Window const& rhs) = delete;
    Window& operator=(Window&& rhs) = delete;

    void init();
    void main_loop();
};
