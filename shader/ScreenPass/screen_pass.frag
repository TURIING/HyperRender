#version 450

layout (location = 0) in vec2 vTexCoord;
layout (binding = 1) uniform sampler2D screenTex;
layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(screenTex, vTexCoord));
}