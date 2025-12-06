//
// Created by turiing on 2025/12/5.
//

#include "EmbossFilter.h"
#include "../../../pass/effect/filter/EmbossFilterPass.h"

USING_RENDER_NAMESPACE_BEGIN

EmbossFilter::EmbossFilter(HyperGpu::GpuDevice *pDevice): BaseEffect(pDevice) {
    m_pEmbossFilterPass = new EmbossFilterPass(pDevice);
}

EmbossFilter::~EmbossFilter() {
    m_pEmbossFilterPass->SubRef();
}

void EmbossFilter::SetEmbossInfo(float azimuth, float elevation, float bumpHeight, bool useGray) {
    m_pEmbossFilterPass->SetLocalInfo(azimuth, elevation, bumpHeight, useGray);
}

void EmbossFilter::DoRender(HyperGpu::GpuCmd *pCmd) {
    const auto size = m_pTargetUnit->GetSize();
    pCmd->BeginDebugUtilsLabel("EmbossFilter::DoRender");
    m_pEmbossFilterPass->SetTargetImage(m_pTargetUnit->GetImage());

    HyperGpu::BeginRenderInfo beginInfo {
        .pPipeline = m_pEmbossFilterPass->GetPipeline(),
        .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
        .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
        .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
        .renderAttachment = {1, (HyperGpu::Image2D*[])m_pResultUnit->GetImage()},
    };
    pCmd->BeginRenderPass(beginInfo);
    pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
    pCmd->SetScissor({0, 0, size.width, size.height});
    m_pEmbossFilterPass->Draw(pCmd);
    pCmd->EndRenderPass();

    pCmd->EndDebugUtilsLabel();
}

USING_RENDER_NAMESPACE_END
