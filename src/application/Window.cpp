#include "Window.h"

#include <iostream>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO: create callback data to access here if needed -> this is the
    // interface between the window callback and the rest of the code auto*
    // auto* callbackData = (CallbackData*) glfwGetWindowUserPointer(window);

    // TODO: remove this template code, only here for future reference
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "I'm in space\n";
    }
}

static void framebufferResizeCallback(GLFWwindow* glfWwindow, int width, int height) {
    // TODO: use callback data to trigger a resize for Vulkan
    // TODO: update width and height of window
    // auto* callbackData = (CallbackData*) glfwGetWindowUserPointer(window);
}

Window::Window(int width, int height, std::string appName)
    : width(width)
    , height(height)
    , appName(appName) {
    glfwInit();

    // required when using Vulkan as GLFW was originally created for OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
}

Window::~Window() {
    glfwDestroyWindow(window);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
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
