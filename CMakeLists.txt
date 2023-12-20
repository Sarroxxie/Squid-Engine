cmake_minimum_required(VERSION 3.24)
project(ThesisRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThesisRenderer src/main.cpp)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})

# Constants for lib includes
set(GLFW_DIR libs/glfw/)
set(IMGUI_DIR libs/imgui/)
set(GLM_DIR libs/glm/)

# TODO: see if we need this for model loading
# set(TINY_GLTF_DIR libs/tiny_gltf/)

# VulkanSDK
find_package(Vulkan REQUIRED)

# includes
include_directories(${PROJECT_ROOT_DIR}/src)
include_directories(${IMGUI_DIR})

# TODO: find out if PRIVATE has to be changed to PUBLIC
target_sources(ThesisRenderer PRIVATE
"${IMGUI_DIR}/imgui.h"
"${IMGUI_DIR}/imgui.cpp"

"${IMGUI_DIR}/imgui_demo.cpp"
"${IMGUI_DIR}/imgui_draw.cpp"
"${IMGUI_DIR}/imgui_tables.cpp"
"${IMGUI_DIR}/imgui_widgets.cpp"

"${IMGUI_DIR}/imgui_impl_vulkan.cpp"
"${IMGUI_DIR}/imgui_impl_glfw.cpp"
)

# We only use glfw, no need to test it or build the docs for us
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add and link libs
add_subdirectory(${GLFW_DIR})
target_link_libraries(ThesisRenderer glfw ${GLFW_LIBRARIES})

target_link_libraries(ThesisRenderer Vulkan::Vulkan)

add_subdirectory(${GLM_DIR})
target_link_libraries(ThesisRenderer glm)