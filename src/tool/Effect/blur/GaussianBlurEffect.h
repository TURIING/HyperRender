//
// Created by turiing on 2025/12/6.
//

#ifndef GAUSSIANBLUREFFECT_H
#define GAUSSIANBLUREFFECT_H

#include "../../../common/common.h"
#include "Effect/blur/IGaussianBlur.h"
#include "../BaseEffect.h"

USING_RENDER_NAMESPACE_BEGIN
class GaussianBlurPass;

class GaussianBlurEffect: public BaseEffect, public IGaussianBlur {
public:
    explicit GaussianBlurEffect(HyperGpu::GpuDevice* pDevice);
    ~GaussianBlurEffect() override;
    void SetBlurInfo(float radius) override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;
    void SetTargetUnit(DrawUnit *pUnit) override;

private:
    GaussianBlurPass* m_pGaussianBlurPass = nullptr;
    HyperGpu::Image2D* m_pTmpImage = nullptr;
    HyperGpu::Sampler* m_pLinearSampler = nullptr;
    float m_radius = 0.0;
};

USING_RENDER_NAMESPACE_END

#endif //GAUSSIANBLUREFFECT_H
