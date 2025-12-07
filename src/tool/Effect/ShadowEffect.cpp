//
// Created by turiing on 2025/12/7.
//

#include "ShadowEffect.h"
#include "../../pass/effect/shadow/AlphaMaskPass.h"
#include "../../pass/effect/shadow/ShadowEffectPass.h"
#include "blur/GaussianBlurEffect.h"
#include "../../common/GpuHelper.h"
#include "../../common/DrawUnit.h"

USING_RENDER_NAMESPACE_BEGIN
ShadowEffect::ShadowEffect(HyperGpu::GpuDevice *pDevice): BaseEffect(pDevice) {
    m_pAlphaMaskPass = new AlphaMaskPass(pDevice);
    m_pShadowEffectPass = new ShadowEffectPass(pDevice);
    m_pLinearSampler = GpuHelper::CreateSampler(m_pGpuDevice);
    m_pGaussianBlurEffect = new GaussianBlurEffect(m_pGpuDevice);
}

ShadowEffect::~ShadowEffect() {
    m_pAlphaMaskPass->SubRef();
    m_pShadowEffectPass->SubRef();
    m_pLinearSampler->SubRef();
    m_pGaussianBlurEffect->SubRef();
    if (m_pAlphaMaskUnit) m_pAlphaMaskUnit->SubRef();
    if (m_pBlurUnit) m_pBlurUnit->SubRef();
}

void ShadowEffect::DoRender(HyperGpu::GpuCmd *pCmd) {
    const auto size = m_pTargetUnit->GetSize();
    // 提取alpha mask
    {
        pCmd->BeginDebugUtilsLabel("EmbossFilter::DoRender AlphaMask");
        m_pAlphaMaskPass->SetTargetImage(m_pTargetUnit->GetImage());

        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pAlphaMaskPass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, (HyperGpu::Image2D*[])m_pAlphaMaskUnit->GetImage()},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pAlphaMaskPass->Draw(pCmd);
        pCmd->EndRenderPass();
        pCmd->EndDebugUtilsLabel();
    }

    // 高斯模糊
    {
        m_pGaussianBlurEffect->SetTargetUnit(m_pAlphaMaskUnit);
        m_pGaussianBlurEffect->SetResultUnit(m_pBlurUnit);
        m_pGaussianBlurEffect->DoRender(pCmd);
    }

    // 阴影偏移处理
    {
        pCmd->BeginDebugUtilsLabel("EmbossFilter::DoRender ShadowOffset");
        m_pShadowEffectPass->SetTargetImage(m_pTargetUnit->GetImage());
        m_pShadowEffectPass->SetAlphaMask(m_pBlurUnit->GetImage());

        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pShadowEffectPass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, (HyperGpu::Image2D*[])m_pResultUnit->GetImage()},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pShadowEffectPass->Draw(pCmd);
        pCmd->EndRenderPass();
        pCmd->EndDebugUtilsLabel();
    }
}

void ShadowEffect::SetShadowInfo(const Color &color, const Offset2D& offset, float soft) {
    m_pAlphaMaskPass->SetLocalInfo(color);
    m_pShadowEffectPass->SetLocalInfo(offset);
    m_pGaussianBlurEffect->SetBlurInfo(soft);
}

void ShadowEffect::SetTargetUnit(DrawUnit *pUnit) {
    if (!m_pAlphaMaskUnit || m_pAlphaMaskUnit->GetSize() != pUnit->GetSize()) {
        if (m_pAlphaMaskUnit) {
            m_pAlphaMaskUnit->SubRef();
            m_pBlurUnit->SubRef();
        }
        DrawUnit::DrawUnitCreateInfo info{};
        info.area = pUnit->GetArea();
        info.pSampler = m_pLinearSampler;
        m_pAlphaMaskUnit = new DrawUnit(m_pGpuDevice, info);
        m_pBlurUnit = new DrawUnit(m_pGpuDevice, info);
    }
    BaseEffect::SetTargetUnit(pUnit);
}

USING_RENDER_NAMESPACE_END
