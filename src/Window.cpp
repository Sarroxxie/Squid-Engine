#include "Window.h"
#include <iostream>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO: create callback data to access here if needed -> this is the
    // interface between the window callback and the rest of the code auto*
    // callbackData = (CallbackData*) glfwGetWindowUserPointer(window);

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "I'm in space\n";
    }
}

static void framebufferResizeCallback(GLFWwindow* glfWwindow, int width, int height) {
    // TODO: use callback data to trigger a resize for Vulkan
    // auto* callbackData = (CallbackData*) glfwGetWindowUserPointer(window);
}

Window::Window(int width, int height, std::string appName)
    : width(width)
    , height(height)
    , appName(appName) {
    initGLFW();
}

Window::~Window() {
    glfwDestroyWindow(window);
}

void Window::initGLFW() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
}

int Window::getWidth() {
    return width;
}

int Window::getHeight() {
    return height;
}

GLFWwindow* Window::getWindowHandle() {
    return this->window;
}
