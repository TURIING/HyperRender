#version 450

layout(binding = 0) uniform sampler2D texture0;
layout (binding = 1) uniform LocalInfo {
    vec2 iResolution;
    vec2 offset;
} localInfo;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec2 uv = vec2(gl_FragCoord.xy / localInfo.iResolution.xy);
    vec2 halfpixel = 0.5 / localInfo.iResolution.xy;

    vec4 sum = texture(texture0, uv + vec2(-halfpixel.x * 2.0, 0.0) * localInfo.offset);            // halfpixel.x * 2.0是为了偏移一个像素
    sum += texture(texture0, uv + vec2(-halfpixel.x, halfpixel.y) * localInfo.offset) * 2.0;
    sum += texture(texture0, uv + vec2(0.0, halfpixel.y * 2.0) * localInfo.offset);
    sum += texture(texture0, uv + vec2(halfpixel.x, halfpixel.y) * localInfo.offset) * 2.0;
    sum += texture(texture0, uv + vec2(halfpixel.x * 2.0, 0.0) * localInfo.offset);
    sum += texture(texture0, uv + vec2(halfpixel.x, -halfpixel.y) * localInfo.offset) * 2.0;
    sum += texture(texture0, uv + vec2(0.0, -halfpixel.y * 2.0) * localInfo.offset);
    sum += texture(texture0, uv + vec2(-halfpixel.x, -halfpixel.y) * localInfo.offset) * 2.0;

    FragColor = sum / 12.0;
}