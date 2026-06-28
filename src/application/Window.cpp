#include "Window.h"

#include "output/Logger.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO: create callback data to access here if needed -> this is the
    // interface between the window callback and the rest of the code auto*
    // auto* callbackData = (CallbackData*) glfwGetWindowUserPointer(window);

    // TODO: remove this template code, only here for future reference
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "I'm in space\n";
    }
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    // TODO: use callback data to trigger a resize for Vulkan
    // TODO: update width and height of window
    // auto* callbackData = (CallbackData*) glfwGetWindowUserPointer(window);
}

Window::Window(const int width, const int height, const std::string appName)
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

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

GLFWwindow* Window::getWindowHandle() const {
    return window;
}

void Window::cleanup() {
    glfwDestroyWindow(window);

    SLOG_INFO("Window cleanup complete.");
}