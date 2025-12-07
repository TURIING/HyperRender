//
// Created by turiing on 2025/12/4.
//

#include "EffectFactory.h"
#include "blur/DualKawaseBlurEffect.h"
#include "blur/GaussianBlurEffect.h"
#include "filter/EmbossFilter.h"
#include "warp/LiquifyEffect.h"

USING_RENDER_NAMESPACE_BEGIN
EffectFactory::EffectFactory(HyperGpu::GpuDevice *pGpuDevice): m_pGpuDevice(pGpuDevice) {
    m_pGpuDevice->AddRef();
}

EffectFactory::~EffectFactory() {
    m_pGpuDevice->SubRef();
}

IDualKawaseBlur * EffectFactory::CreateDualKawaseBlurEffect() {
    return new DualKawaseBlurEffect(m_pGpuDevice);
}

ILiquifyEffect * EffectFactory::CreateLiquifyEffect() {
    return new LiquifyEffect(m_pGpuDevice);
}

IEmbossFilter * EffectFactory::CreateEmbossFilter() {
    return new EmbossFilter(m_pGpuDevice);
}

IGaussianBlur * EffectFactory::CreateGaussianBlur() {
    return new GaussianBlurEffect(m_pGpuDevice);
}

USING_RENDER_NAMESPACE_END
