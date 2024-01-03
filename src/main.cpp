#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// how to include the Vulkan Memory Allocator VMA (if it is installed with the
// VulkanSDK) #include <vma/vk_mem_alloc.h>

#include "application/Window.h"
#include "application/DefaultApplication.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr char APP_NAME[] = "ThesisRenderer";

int main() {
    DefaultApplication app;

    /* TODO: roadmap
     *
     * 1. physical and logical device
     * 2. implement swap chaip chain
     * 3. implement graphics pipeline + render pass
     * 4. add ImGui (maybe set context per Window)
     * 5. Hello Triangle
     * 6. Shadertoy (use Text Editor Widget from ImGui?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    while(!window.shouldClose()) {
        // main loop
        glfwPollEvents();
    }
    window.~Window();

    app.cleanup();
    glfwTerminate();
    return 0;
}