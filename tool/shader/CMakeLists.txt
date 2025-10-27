cmake_minimum_required(VERSION 3.26)
project(ShaderCompiler)

set(PATH_BIN "$ENV{VULKAN_SDK}/bin")
set(GLSL_LANG_VALIDATOR "${PATH_BIN}/glslangValidator")
set(SPIRV_CROSS "${PATH_BIN}/spirv-cross")
set(PATH_SHADER_INPUT "../../shader")
set(PATH_SHADER_OUTPUT "../../src/shader")
set(PATH_SHADER_OUTPUT_TEMP "${CMAKE_SOURCE_DIR}/output")
set(PATH_SHADER_INCLUDE "${CMAKE_SOURCE_DIR}/../../shader/include")

include(cmake/CompileShaderToHeader.cmake)

COMPILE_ALL_SHADER(${PATH_SHADER_INPUT} ${PATH_SHADER_OUTPUT_TEMP})
TRANSFORM_ALL_RESULT(${PATH_SHADER_OUTPUT_TEMP} ${PATH_SHADER_OUTPUT})