#version 450
layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 FragColor;

layout(set = 0, binding = 1) uniform sampler2D targetTex;
layout(set = 0, binding = 2) uniform sampler2D uvReadTex;

void main() {
    vec2 uv = texture(uvReadTex, texCoord).xy;
    FragColor = texture(targetTex, uv);
}