//
// Created by turiing on 2025/12/7.
//

#ifndef SHADOWEFFECT_H
#define SHADOWEFFECT_H

#include "BaseEffect.h"
#include "Effect/IShadowEffect.h"

USING_RENDER_NAMESPACE_BEGIN
class AlphaMaskPass;
class ShadowEffectPass;
class GaussianBlurEffect;
class DrawUnit;

class ShadowEffect: public BaseEffect, public IShadowEffect {
public:
    explicit ShadowEffect(HyperGpu::GpuDevice* pDevice);
    ~ShadowEffect() override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;
    void SetShadowInfo(const Color &color, const Offset2D& offset, float soft) override;
    void SetTargetUnit(DrawUnit *pUnit) override;

private:
    GaussianBlurEffect* m_pGaussianBlurEffect = nullptr;
    HyperGpu::Sampler* m_pLinearSampler = nullptr;
    AlphaMaskPass* m_pAlphaMaskPass = nullptr;
    ShadowEffectPass* m_pShadowEffectPass = nullptr;
    DrawUnit* m_pAlphaMaskUnit = nullptr;
    DrawUnit* m_pBlurUnit = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SHADOWEFFECT_H
