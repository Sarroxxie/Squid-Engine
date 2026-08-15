#include "ShaderCompiler.h"

#include "output/Logger.h"

bool CommandLineShaderCompiler::compile(const std::filesystem::path sourceFilePath,
                                        const std::vector<ShaderEntryPoint> entryPoints) const {
    // TODO: need to generalize the command

    /*
    example command (see
    https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/01_Shader_modules.html):

    slangc shader.slang -target spirv -profile spirv_1_4 -emit-spirv-directly
           -fvk-use-entrypoint-name -entry vertMain -entry fragMain -o slang.spv
    */

    // compile command
    std::string command = "slangc";

    // input file
    command += " " + std::string(SHADER_SOURCE_DIRECTORY_PATH) + sourceFilePath.string();

    // specify spirv as target
    command += " -target spirv -profile spirv_1_4 -emit-spirv-directly";

    // add entry points
    command += " -fvk-use-entrypoint-name";
    for(ShaderEntryPoint entryPoint : entryPoints) {
        command += " -entry " + entryPoint.name;
    }

    // specify output file
    command += " -o " + std::string(SHADER_COMPILE_DIRECTORY_PATH)
               + sourceFilePath.parent_path().string()
               + sourceFilePath.stem().string() + ".spv";


    SLOG_INFO("Compiling Shader \"" << sourceFilePath.string() << "\"");
    int result = system(command.c_str());

    if(result == 0) {
        return true;
    }
    SLOG_ERROR("Compilation of Shader \"" << sourceFilePath.string() << "\" failed.");
    return false;
}