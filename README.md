# Squid Engine

The goal of this project is to develop an easily adjustable Vulkan renderer that can be used as a starting point to implement fancy graphics stuff.

## Setup

### Cloning
This repository contains [submodules](https://git-scm.com/book/de/v2/Git-Tools-Submodule) for some of its libraries. Cloning it does not automatically clone its submodules. 

Either clone it with the ```"--recurse-submodules"``` flag or, if the project is already cloned, run these two commands:
```
git submodule init
git submodule update
```

### Building
The whole build process is being executed when calling **cmake** in the root directory. It is recommended to create a **/build** directory inside the root directory and to call ```"cmake .."``` from inside the build directory. This way all the built files will be written into **/build** and are also already being excluded from the repository via .gitignore.

### VulkanSDK
CMake will automatically try to grab the VulkanSDK path via ```"find_package(Vulkan REQUIRED)"```. For this to work, the environment variable **VULKAN_SDK** has to point to a VulkanSDK installation.  
This project uses the VulkanSDK ([official download page](https://vulkan.lunarg.com/sdk/home)). The version this project got developed on is the **VulkanSDK 1.4.335.0**.
