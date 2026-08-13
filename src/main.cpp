#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// how to include the Vulkan Memory Allocator VMA (if it is installed with the
// VulkanSDK)
// #include <vma/vk_mem_alloc.h>

#include "application/Application.h"

constexpr int  WIDTH      = 1280;
constexpr int  HEIGHT     = 720;
constexpr char APP_NAME[] = "Squid Engine";

int main() {
    /* TODO: roadmap
     *
     * 2.5. write shader auto compiler
     * 3. implement graphics pipeline + render pass
     * 4. Hello Triangle
     * 5. add ImGui (maybe set context per Window)
     * 6. Shadertoy (use Text Editor Widget from ImGui?)
     *
     *
     * -> also split application and rendering! application should handle inputs
     * (and physics?)
     */

    Window window(WIDTH, HEIGHT, APP_NAME);

    DefaultVulkanRenderer renderer(&window, true);

    Application app(&renderer);

    app.run();

    app.cleanup();
    glfwTerminate();
    return 0;
}