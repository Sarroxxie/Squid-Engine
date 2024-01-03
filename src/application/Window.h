#pragma once

#include <string>
#include <glfw/glfw3.h>

/*
This class handles window creation and destruction. As long as the
"shouldClose()"-method returns false, the window is open.
*/
class Window
{
  public:
    Window(int width, int height, std::string application_name);
    ~Window();
    // Wrapper for "glfwWindowShouldClose(..)".
    bool shouldClose();

    int         getWidth();
    int         getHeight();
    GLFWwindow* getWindowHandle();

  private:
    int         width;
    int         height;
    std::string appName;
    GLFWwindow* window;
};