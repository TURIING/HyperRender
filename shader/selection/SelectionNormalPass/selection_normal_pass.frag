// https://www.shadertoy.com/view/4lKXWD
#version 450
#extension GL_GOOGLE_include_directive:enable

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform LocalInfo {
    vec4 color;         // 蚂蚁线的颜色
    vec2 rectMin;       // 区域左上角的纹理坐标
    vec2 rectMax;       // 区域右下角的纹理坐标
    float time;         // 流逝的时间，用于做动画
    float density;      // 蚂蚁线密度
} localInfo;

void main()
{
    // 1. 从 UV 空间开始
    vec2 uv = texCoord;

    // 2. 定义矩形范围
    vec2 center   = (localInfo.rectMin + localInfo.rectMax) * 0.5;
    vec2 halfSize = center - localInfo.rectMin;

    // 3. fwidth 在 GL3.0+ 中可直接使用
    vec2 fw = fwidth(uv);
    vec2 dist = abs(uv - center);

    vec4 color = vec4(0.0);

    // 4. 边缘区域检测（矩形内部 && 临近边界）
    bool inside = all(lessThan(dist, halfSize));
    bool nearEdge = any(greaterThan(dist, halfSize - fw));

    if (inside && nearEdge)
    {
        // 5. 构造“像素级”坐标
        vec2 pixel = uv / fw;

        float aspect = halfSize.y / halfSize.x;

        // 判断像素靠哪条边 → 决定虚线方向
        float dir = (dist.x * aspect > dist.y) ? -sign(uv.x - center.x) : sign(uv.y - center.y);

        // 6. 虚线公式：周期 + 动画
        float dash = step(0.5, fract((pixel.x + pixel.y) * dir / localInfo.density + localInfo.time));

        color = mix(vec4(0.0), localInfo.color, dash);
    }

    fragColor = color;
}