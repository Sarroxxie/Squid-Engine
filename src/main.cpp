#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// how to include the Vulkan Memory Allocator VMA (if it is installed with the VulkanSDK)
// #include <vma/vk_mem_alloc.h>

#include "application/Window.h"
#include "application/DefaultApplication.h"

#define WIDTH 1280
#define HEIGHT 720
#define APP_NAME "ThesisRenderer"

int main() {
    DefaultApplication app;

    /* TODO: roadmap
     *
     * 0. Debug Messenger for Validation Layers
     * 
     * 1. work through Vulkan tutorial to understand how rendering one frame works
     * 2. implement swap chaip chain
     * 3. implement graphics pipeline + render pass
     * 4. add ImGui (maybe set context per Window)
     * 5. Hello Triangle
     * 6. Shadertoy (use Text Editor Widget from ImGui?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    while(!glfwWindowShouldClose(window.getWindowHandle())) {
        // main loop
        glfwPollEvents();
    }

    app.cleanup();
    glfwTerminate();
    return 0;
}