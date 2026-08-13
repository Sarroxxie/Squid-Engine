#include "Application.h"

#include "output/Logger.h"

Application::Application(DefaultVulkanRenderer* const renderer) : renderer(renderer) {}

void Application::cleanup() {
    renderer->cleanup();
}

void Application::run() {
    while(!renderer->getWindow()->shouldClose()) {
        // main loop
        processFrame();
    }
}

void Application::processFrame() {
    glfwPollEvents();

    // TODO: do physics stuff, update game objects

    // TODO: render frame
    // renderer.render();
}
