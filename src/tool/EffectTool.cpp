/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/1 22:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "EffectTool.h"
#include "../pass/effect/RoundCornerPass.h"
#include "../pass/effect/DualKawaseBlurDownSamplePass.h"
#include "../pass/effect/DualKawaseBlurUpSamplePass.h"
#include "../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {
    // m_pRoundCornerPass = new RoundCornerPass(m_pGpuDevice);
    m_pDualKawaseBlurUpSamplePass = new DualKawaseBlurUpSamplePass(m_pGpuDevice);
    m_pDualKawaseBlurDownSamplePass = new DualKawaseBlurDownSamplePass(m_pGpuDevice);
}

EffectTool::~EffectTool() {
    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    // m_pRoundCornerPass->SubRef();
    m_pDualKawaseBlurUpSamplePass->SubRef();
    m_pDualKawaseBlurDownSamplePass->SubRef();
}

void EffectTool::SetRoundCorner(float radius) {
}

void EffectTool::Begin(const Area &renderArea) {
    m_renderArea = renderArea;
    begin();
}

void EffectTool::DoDualKawaseBlur(int interation, const Offset2D &offset) {
    m_pDualKawaseBlurUpSamplePass->SetOffset(offset);
    m_pDualKawaseBlurDownSamplePass->SetOffset(offset);

    std::vector<HyperGpu::Image2D*> vecImg;
    vecImg.push_back(m_pResultUnit->GetImage());
    const auto targetSize = m_pTargetUnit->GetSize();
    for (auto i = 1; i < interation + 1; i++) {
        auto name = std::format("EffectToolDkBlur{}", i);
        auto pImage = GpuHelper::CreateImage(m_pGpuDevice, targetSize/pow(2,i), m_pCommonSampler, name.c_str());
        vecImg.push_back(pImage);
    }

    GpuHelper::CopyImage(m_pGpuDevice, m_pTargetUnit->GetImage(), vecImg[0]);

    for (auto i = 0; i < interation; i++) {
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
        m_pCmd->BeginRenderPass(beginInfo);
        m_pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        m_pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurDownSamplePass->Draw(m_pCmd, i);
        m_pCmd->EndRenderPass();
    }

    for (auto i = interation; i > 0; i--) {
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
        m_pCmd->BeginRenderPass(beginInfo);
        m_pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        m_pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurUpSamplePass->Draw(m_pCmd, interation - i);
        m_pCmd->EndRenderPass();
    }
}

void EffectTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = dynamic_cast<DrawUnit *>(pTargetUnit);

    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    m_pResultUnit = BaseTool::CreateDrawUnit(m_pTargetUnit->GetArea(), "EffectToolResultUnit");
}

void EffectTool::End() {
    end();
}

void EffectTool::RenderToUnit(IDrawUnit *resultUnit) {
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END