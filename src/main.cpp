#include <GLFW/glfw3.h>
#include <iostream>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* callbackData = glfwGetWindowUserPointer(window);

    // VulkanRenderer* renderer = (VulkanRenderer*)glfwGetWindowUserPointer(window);
    // compile shaders
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "space was pressed\n";
    }
}

static void framebufferResizeCallback(GLFWwindow* glfWwindow, int width, int height) {
	std::cout << "resize got called\n";
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "ThesisRenderer", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	
	// clean up resources
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}