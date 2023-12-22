#pragma once

#include <string>
#include <glfw/glfw3.h>

class Window
{
  private:
    int         width;
    int         height;
    std::string appName;
    GLFWwindow* window;

  public:
    Window(int width, int height, std::string application_name);
    ~Window();

    int getWidth();
    int getHeight();

    GLFWwindow* getWindowHandle();
};