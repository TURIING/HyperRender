#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D targetTex;

layout(set = 0, binding = 1) uniform LocalInfo {
    vec3 shadowColor;
    float shadowAlpha;
} localInfo;

void main() {
    vec4 src = texture(targetTex, vUV);
    float a = src.a;
    FragColor = vec4(localInfo.shadowColor, localInfo.shadowAlpha * a);
}