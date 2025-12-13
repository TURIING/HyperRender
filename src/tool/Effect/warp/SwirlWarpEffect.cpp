//
// Created by turiing on 2025/12/13.
//

#include "SwirlWarpEffect.h"
#include "../../../pass/effect/warp/SwirlWarpEffectPass.h"

USING_RENDER_NAMESPACE_BEGIN
SwirlWarpEffect::SwirlWarpEffect(HyperGpu::GpuDevice *pDevice): BaseEffect(pDevice) {
    m_pSwirlWarpEffectPass = new SwirlWarpEffectPass(pDevice);
}

SwirlWarpEffect::~SwirlWarpEffect() {
    m_pSwirlWarpEffectPass->SubRef();
}

void SwirlWarpEffect::SetSwirlInfo(const PointI &center, float angle, float radius) {
    m_pSwirlWarpEffectPass->SetLocalInfo(radius, angle, { center.x, center.y });
}

void SwirlWarpEffect::DoRender(HyperGpu::GpuCmd *pCmd) {
    const auto size = m_pTargetUnit->GetSize();
    pCmd->BeginDebugUtilsLabel("SwirlWarpEffect::DoRender");
    m_pSwirlWarpEffectPass->SetTargetImage(m_pTargetUnit->GetImage());

    HyperGpu::BeginRenderInfo beginInfo {
        .pPipeline = m_pSwirlWarpEffectPass->GetPipeline(),
        .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
        .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
        .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
        .renderAttachment = {1, (HyperGpu::Image2D*[])m_pResultUnit->GetImage()},
    };
    pCmd->BeginRenderPass(beginInfo);
    pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
    pCmd->SetScissor({0, 0, size.width, size.height});
    m_pSwirlWarpEffectPass->Draw(pCmd);
    pCmd->EndRenderPass();

    pCmd->EndDebugUtilsLabel();
}

USING_RENDER_NAMESPACE_END
