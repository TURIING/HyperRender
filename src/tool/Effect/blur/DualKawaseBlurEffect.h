//
// Created by turiing on 2025/12/4.
//

#ifndef DUALKAWASEBLUREFFECT_H
#define DUALKAWASEBLUREFFECT_H

#include "../../../common/common.h"
#include "Effect/blur/IDualKawaseBlur.h"
#include "../BaseEffect.h"

USING_RENDER_NAMESPACE_BEGIN
class DualKawaseBlurDownSamplePass;
class DualKawaseBlurUpSamplePass;

class DualKawaseBlurEffect : public IDualKawaseBlur, public BaseEffect {
public:
    explicit DualKawaseBlurEffect(HyperGpu::GpuDevice* pGpuDevice);
    ~DualKawaseBlurEffect() override;
    /**
     * DualKawaseBlur
     * @param interation 迭代次数
     * @param offset 偏移
     */
    void SetBlurInfo(int interation, const Offset2D &offset) override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;

private:
    DualKawaseBlurUpSamplePass* m_pDualKawaseBlurUpSamplePass = nullptr;
    DualKawaseBlurDownSamplePass* m_pDualKawaseBlurDownSamplePass = nullptr;
    HyperGpu::Sampler* m_pLinearSampler = nullptr;
    int m_interation = 0;
    Offset2D m_offset;
};

USING_RENDER_NAMESPACE_END

#endif //DUALKAWASEBLUREFFECT_H
