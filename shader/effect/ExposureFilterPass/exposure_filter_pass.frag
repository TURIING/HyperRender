#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D uTex;

layout(set = 0, binding = 1) uniform LocalInfo {
    float exposure;     // 曝光度
} localInfo;

vec3 exposureCurve(vec3 c, float e)
{
    return 1.0 - exp(-c * e);
}

void main()
{
    vec4 color = texture(uTex, texCoord);
    color.rgb = exposureCurve(color.rgb, localInfo.exposure);
    FragColor = color;
}