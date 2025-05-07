#version 450

layout (binding = 0) uniform GlobalInfo {
    mat4 view;
    mat4 proj;
} globalInfo;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 0) out vec2 vTexCoord;

void main()
{
    gl_Position = globalInfo.proj * globalInfo.view * vec4(aPos.x, aPos.y, 0.0, 1.0);
    vTexCoord = aTexCoord;
}