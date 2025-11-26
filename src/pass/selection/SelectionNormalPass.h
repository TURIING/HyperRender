//
// Created by turiing on 2025/11/24.
//

#ifndef SELECTIONNORMALPASS_H
#define SELECTIONNORMALPASS_H

#include "../../common/common.h"
#include "../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class SelectionNormalPass : public BasePass {
    struct LocalInfo {
        alignas(16) glm::vec4 color{};         // 蚂蚁线的颜色
        alignas(8) glm::vec2 rectMin{};        // 区域左上角的纹理坐标
        alignas(8) glm::vec2 rectMax{};        // 区域右下角的纹理坐标
        alignas(4) float time = 0;             // 流逝的时间，用于做动画
        alignas(4) float density = 10;         // 蚂蚁线密度
    };
public:
    explicit SelectionNormalPass(HyperGpu::GpuDevice* pDevice);
    void SetLocalInfo(const glm::vec2 &rectMin, const glm::vec2 &rectMax, const glm::vec4 &color, float density = 10, float time = 0);
    void Draw(HyperGpu::GpuCmd *pCmd) override;

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SELECTIONNORMALPASS_H
