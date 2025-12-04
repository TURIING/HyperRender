//
// Created by turiing on 2025/12/4.
//

#ifndef LIQUIFYEFFECT_H
#define LIQUIFYEFFECT_H

#include "../../../common/common.h"
#include "Effect/warp/ILiquifyEffect.h"
#include "../BaseEffect.h"

USING_RENDER_NAMESPACE_BEGIN
class LiquifyMainPass;
class LiquifyImagePass;

class LiquifyEffect : public ILiquifyEffect, public BaseEffect {
public:
    explicit LiquifyEffect(HyperGpu::GpuDevice* pGpuDevice);
    ~LiquifyEffect() override;
    void SetLiquifyInfo(const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;

private:
    LiquifyMainPass* m_pLiquifyMainPass = nullptr;
    LiquifyImagePass* m_pLiquifyImagePass = nullptr;
    std::vector<HyperGpu::Image2D*> m_vecLiquifyImage;
    HyperGpu::Sampler* m_pLinearSampler = nullptr;
    PointI m_newMousePos;
    PointI m_oldMousePos;
    bool m_isPressed = false;
    bool m_isFirstFrame = false;
};

USING_RENDER_NAMESPACE_END

#endif //LIQUIFYEFFECT_H
