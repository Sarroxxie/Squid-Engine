#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"

#define WIDTH 1280
#define HEIGHT 720
#define APP_NAME "ThesisRenderer"

int main() {
    Window window(WIDTH, HEIGHT, APP_NAME);

	while (!glfwWindowShouldClose(window.getWindowHandle())) {
		glfwPollEvents();
	}
	return 0;
}