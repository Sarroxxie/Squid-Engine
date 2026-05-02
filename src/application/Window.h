#pragma once

#include <string>
// this has to be defined before glfw3.h is included so the methods necessary
// for usage with Vulkan get loaded
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

// TODO: find another location for these callbacks to make them customizable (should be tied to the Application)
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void framebufferResizeCallback(GLFWwindow* glfWwindow, int width, int height);

/*
This class handles window creation and destruction. As long as the
"shouldClose()"-method returns false, the window is open.
*/
class Window
{
  public:
    Window(const int width, const int height, const std::string application_name);
    ~Window();
    // Wrapper for "glfwWindowShouldClose(..)".
    bool shouldClose() const;

    int         getWidth() const;
    int         getHeight() const;
    GLFWwindow* getWindowHandle() const;

  private:
    int         width;
    int         height;
    std::string appName;
    GLFWwindow* window;
};