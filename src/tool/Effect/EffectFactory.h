//
// Created by turiing on 2025/12/4.
//

#ifndef EFFECTFACTORY_H
#define EFFECTFACTORY_H

#include "Effect/IEffectFactory.h"
#include "../../common/common.h"

USING_RENDER_NAMESPACE_BEGIN

class EffectFactory: virtual public IEffectFactory {
public:
    explicit EffectFactory(HyperGpu::GpuDevice* pGpuDevice);
    ~EffectFactory() override;
    NODISCARD IDualKawaseBlur * CreateDualKawaseBlurEffect() override;
    NODISCARD ILiquifyEffect * CreateLiquifyEffect() override;
    NODISCARD IEmbossFilter * CreateEmbossFilter() override;
    NODISCARD IGaussianBlur * CreateGaussianBlur() override;
    NODISCARD IShadowEffect * CreateShadowEffect() override;
    NODISCARD IExposureFilter * CreateExposureFilter() override;
    NODISCARD ISwirlWarpEffect* CreateSwirlWarpEffect() override;

private:
    HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //EFFECTFACTORY_H
