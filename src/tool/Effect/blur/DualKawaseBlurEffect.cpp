//
// Created by turiing on 2025/12/4.
//

#include "DualKawaseBlurEffect.h"
#include "../../../pass/effect/DualKawaseBlurDownSamplePass.h"
#include "../../../pass/effect/DualKawaseBlurUpSamplePass.h"
#include "../../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN
DualKawaseBlurEffect::DualKawaseBlurEffect(HyperGpu::GpuDevice* pGpuDevice): BaseEffect(pGpuDevice) {
    m_pDualKawaseBlurUpSamplePass = new DualKawaseBlurUpSamplePass(m_pGpuDevice);
    m_pDualKawaseBlurDownSamplePass = new DualKawaseBlurDownSamplePass(m_pGpuDevice);
    m_pLinearSampler = GpuHelper::CreateSampler(pGpuDevice);
}

DualKawaseBlurEffect::~DualKawaseBlurEffect() {
    m_pDualKawaseBlurUpSamplePass->SubRef();
    m_pDualKawaseBlurDownSamplePass->SubRef();
}

void DualKawaseBlurEffect::SetBlurInfo(int interation, const Offset2D &offset) {
    m_interation = interation;
    m_offset = offset;
}

void DualKawaseBlurEffect::DoRender(HyperGpu::GpuCmd *pCmd) {
	pCmd->BeginDebugUtilsLabel("DualKawaseBlurEffect::DoRender");
    m_pDualKawaseBlurUpSamplePass->SetOffset(m_offset);
    m_pDualKawaseBlurDownSamplePass->SetOffset(m_offset);

    std::vector<HyperGpu::Image2D*> vecImg;
    vecImg.push_back(m_pResultUnit->GetImage());
    const auto targetSize = m_pTargetUnit->GetSize();
    for (auto i = 1; i < m_interation + 1; i++) {
        auto name = std::format("EffectToolDkBlur{}", i);
        auto pImage = GpuHelper::CreateImage(m_pGpuDevice, targetSize/pow(2,i), m_pLinearSampler, name.c_str());
        vecImg.push_back(pImage);
    }

    GpuHelper::CopyImage(m_pGpuDevice, pCmd, m_pTargetUnit->GetImage(), vecImg[0]);

    for (auto i = 0; i < m_interation; i++) {
        auto image = vecImg[i+1];
        const auto imageSize = image->GetSize();
        m_pDualKawaseBlurDownSamplePass->SetTargetTexture(vecImg[i]);
        m_pDualKawaseBlurDownSamplePass->SetScreenSize(std::bit_cast<Size>(imageSize));
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pDualKawaseBlurDownSamplePass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, imageSize},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, &image},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurDownSamplePass->Draw(pCmd, i);
        pCmd->EndRenderPass();
    }

    for (auto i = m_interation; i > 0; i--) {
        auto image = vecImg[i-1];
        const auto imageSize = image->GetSize();
        m_pDualKawaseBlurUpSamplePass->SetTargetTexture(vecImg[i]);
        m_pDualKawaseBlurUpSamplePass->SetScreenSize(std::bit_cast<Size>(imageSize));
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pDualKawaseBlurUpSamplePass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, imageSize},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, &image},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurUpSamplePass->Draw(pCmd, m_interation - i);
        pCmd->EndRenderPass();
    }
	pCmd->EndDebugUtilsLabel();
}

USING_RENDER_NAMESPACE_END
