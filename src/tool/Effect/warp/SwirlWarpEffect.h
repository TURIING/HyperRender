//
// Created by turiing on 2025/12/13.
//

#ifndef SWIRLWARPEFFECT_H
#define SWIRLWARPEFFECT_H

#include "../../../common/common.h"
#include "Effect/warp/ISwirlWarpEffect.h"
#include "../BaseEffect.h"

USING_RENDER_NAMESPACE_BEGIN
class SwirlWarpEffectPass;

class SwirlWarpEffect: public BaseEffect, public ISwirlWarpEffect {
public:
    explicit SwirlWarpEffect(HyperGpu::GpuDevice* pDevice);
    ~SwirlWarpEffect() override;
    void SetSwirlInfo(const PointI &center, float angle, float radius) override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;

private:
    SwirlWarpEffectPass* m_pSwirlWarpEffectPass = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SWIRLWARPEFFECT_H
