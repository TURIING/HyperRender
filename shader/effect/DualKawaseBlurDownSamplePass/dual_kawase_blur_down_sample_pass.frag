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
    vec2 halfpixel = 0.5 / localInfo.iResolution.xy;                    // 1.0/iResolution表示一个完整的像素间隔, 0.5/iResolution就是半个像素的偏移

    vec4 sum = texture(texture0, uv) * 4.0;                             // 给中心点一个 4倍权重
    sum += texture(texture0, uv - halfpixel.xy * localInfo.offset);     // 在当前像素周围 四个对角方向 做采样
    sum += texture(texture0, uv + halfpixel.xy * localInfo.offset);
    sum += texture(texture0, uv + vec2(halfpixel.x, -halfpixel.y) * localInfo.offset);
    sum += texture(texture0, uv - vec2(halfpixel.x, -halfpixel.y) * localInfo.offset);

    FragColor = sum / 8.0;                                              // 加权和 = 中心点(权重4) + 四个对角(各权重1); 总权重 = 4+1×4 = 8; 所以除以8.0，就把权重归一化成1
}