#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "window/Window.h"
#include "setup/InstanceBuilder.h"

#define WIDTH 1280
#define HEIGHT 720
#define APP_NAME "ThesisRenderer"

int main() {
    InstanceBuilder builder;
    builder.setAppName(APP_NAME).setApiVersion(0, 1, 3);
    VkInstance instance;
    VkResult   result = builder.build(instance);

    /* TODO: roadmap
     *
     * 1. try out vk-bootstrap
     * 2. work through Vulkan tutorial to understand how window creation works
     * 3. work through Vulkan tutorial to understand how rendering one frame works
     * 4. implement swap chain -> change clear color on button press
     * 5. add ImGui (maybe set context per Window)
     * 6. RenderPass + Shaders + Hello Triangle
     * 7. Shadertoy (use Text Editor Widget from ImGui?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    while(!glfwWindowShouldClose(window.getWindowHandle())) {
        // main loop
        glfwPollEvents();
    }

    // TODO: find a better location for the cleanup of all mandatory objects
    // (e.g. Instance, Device, etc.)
    //       -> if we need those for every usecase of the engine, they should be
    //       automatically handled (new class Application?)
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();
    return 0;
}