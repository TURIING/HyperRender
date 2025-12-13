//
// Created by turiing on 2025/12/13.
//

#ifndef SWIRLWARPEFFECTPASS_H
#define SWIRLWARPEFFECTPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class SwirlWarpEffectPass: public BasePass {
    struct LocalInfo {
        alignas(4) float radius;           // 旋转半径，px
        alignas(4) float angle;            // 旋转角度，弧度制
        alignas(8) glm::vec2 center;            // 旋转中心像素坐标
    };
public:
    explicit SwirlWarpEffectPass(HyperGpu::GpuDevice* pDevice);
    ~SwirlWarpEffectPass() override;
    void SetTargetImage(HyperGpu::Image2D *pImage);
    void SetLocalInfo(float radius, float angle, const glm::vec2 &center);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SWIRLWARPEFFECTPASS_H
