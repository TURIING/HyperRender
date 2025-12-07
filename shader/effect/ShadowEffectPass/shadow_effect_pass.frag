#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 vUV;
layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D targetTex;
layout (binding = 1) uniform sampler2D alphaTex;

layout(set = 0, binding = 2) uniform LocalInfo {
    vec2 shadowOffset;
} localInfo;

void main() {
    vec2 texSize = vec2(textureSize(alphaTex, 0));
    vec2 shadowUV = vUV - localInfo.shadowOffset * (1.0/texSize);
    vec4 shadowCol = texture(alphaTex, shadowUV);

    vec4 src = texture(targetTex, vUV);
    FragColor = src * src.a + shadowCol * (1 - src.a);
}