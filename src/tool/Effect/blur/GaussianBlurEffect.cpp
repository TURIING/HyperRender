//
// Created by turiing on 2025/12/6.
//

#include "GaussianBlurEffect.h"
#include "../../../pass/effect/blur/GaussianBlurPass.h"
#include "../../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

GaussianBlurEffect::GaussianBlurEffect(HyperGpu::GpuDevice *pDevice): BaseEffect(pDevice) {
    m_pGaussianBlurPass = new GaussianBlurPass(m_pGpuDevice);
    m_pLinearSampler = GpuHelper::CreateSampler(m_pGpuDevice);
}

GaussianBlurEffect::~GaussianBlurEffect() {
    m_pGaussianBlurPass->SubRef();
    m_pLinearSampler->SubRef();
    if (m_pTmpImage) {
        m_pTmpImage->SubRef();
    }
}

void GaussianBlurEffect::SetBlurInfo(float radius) {
    m_radius = radius;
}

void GaussianBlurEffect::DoRender(HyperGpu::GpuCmd *pCmd) {
    const auto size = m_pTargetUnit->GetSize();

    {
        m_pGaussianBlurPass->SetTargetImage(m_pTargetUnit->GetImage());
        m_pGaussianBlurPass->SetHorizontalLocalInfo(m_radius, false);
        pCmd->BeginDebugUtilsLabel("GaussianBlurEffect::DoRender horizontal");
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pGaussianBlurPass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, (HyperGpu::Image2D*[])m_pTmpImage},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pGaussianBlurPass->Draw(pCmd);
        pCmd->EndRenderPass();
        pCmd->EndDebugUtilsLabel();
    }

    {
        m_pGaussianBlurPass->SetTargetImage(m_pTmpImage);
        m_pGaussianBlurPass->SetVerticalLocalInfo(m_radius, true);
        pCmd->BeginDebugUtilsLabel("GaussianBlurEffect::DoRender vertical");
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pGaussianBlurPass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, (HyperGpu::Image2D*[])m_pResultUnit->GetImage()},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pGaussianBlurPass->Draw(pCmd);
        pCmd->EndRenderPass();
        pCmd->EndDebugUtilsLabel();
    }
}

void GaussianBlurEffect::SetTargetUnit(DrawUnit *pUnit) {
    if (!m_pTmpImage || std::bit_cast<Size>(m_pTmpImage->GetSize()) != pUnit->GetSize()) {
        if (m_pTmpImage) m_pTmpImage->SubRef();
        m_pTmpImage = GpuHelper::CreateImage(m_pGpuDevice, pUnit->GetSize(), m_pLinearSampler, "GaussianBlurEffectTmpImage");
    }
    BaseEffect::SetTargetUnit(pUnit);
}

USING_RENDER_NAMESPACE_END
