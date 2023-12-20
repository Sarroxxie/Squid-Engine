#include <imgui.h>
#include <iostream>

int main() {
	ImGui::Begin("Test Window");
	ImGui::End();
	std::cout << "success\n";
	return 0;
}