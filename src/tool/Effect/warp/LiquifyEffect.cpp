//
// Created by turiing on 2025/12/4.
//

#include "LiquifyEffect.h"
#include "../../../pass/effect/LiquifyMainPass.h"
#include "../../../pass/effect/LiquifyImagePass.h"
#include "../../../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN
LiquifyEffect::LiquifyEffect(HyperGpu::GpuDevice *pGpuDevice): BaseEffect(pGpuDevice) {
    m_pLiquifyMainPass = new LiquifyMainPass(pGpuDevice);
    m_pLiquifyImagePass = new LiquifyImagePass(pGpuDevice);
    m_pLinearSampler = GpuHelper::CreateSampler(pGpuDevice);
}

LiquifyEffect::~LiquifyEffect() {
    m_pLiquifyMainPass->SubRef();
    m_pLiquifyImagePass->SubRef();
}

void LiquifyEffect::SetLiquifyInfo(const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) {
    m_newMousePos = newMousePos;
    m_oldMousePos = oldMousePos;
    m_isPressed = isPressed;
    m_isFirstFrame = firstFrame;
}

void LiquifyEffect::DoRender(HyperGpu::GpuCmd *pCmd) {
    const auto size = m_pTargetUnit->GetSize();
    pCmd->BeginDebugUtilsLabel("LiquifyEffect::DoRender");
    if (m_vecLiquifyImage.empty()) {
        for (auto i = 0; i < 2; i++) {
            m_vecLiquifyImage.push_back(GpuHelper::CreateImage(m_pGpuDevice, size, m_pLinearSampler, nullptr, HyperGpu::PixelFormat::R16G16B16A16_SFLOAT));
            pCmd->ClearColorImage(m_vecLiquifyImage[i], {0, 0, 0, 0});
        }
    }

    {
        m_pLiquifyMainPass->SetLocalInfo(size, m_newMousePos, m_oldMousePos, m_isPressed, m_isFirstFrame);
        m_pLiquifyMainPass->SetUvInputImage(m_vecLiquifyImage[0]);
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pLiquifyMainPass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, &m_vecLiquifyImage[1]},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pLiquifyMainPass->Draw(pCmd);
        pCmd->EndRenderPass();
    }

    {
        m_pLiquifyImagePass->SetTargetImage(m_pTargetUnit->GetImage());
        m_pLiquifyImagePass->SetUvInputImage(m_vecLiquifyImage[1]);
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pLiquifyImagePass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, (HyperGpu::Image2D*[]){m_pResultUnit->GetImage()}},
        };
        pCmd->BeginRenderPass(beginInfo);
        pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
        pCmd->SetScissor({0, 0, size.width, size.height});
        m_pLiquifyImagePass->Draw(pCmd);
        pCmd->EndRenderPass();
    }

    std::swap(m_vecLiquifyImage[0], m_vecLiquifyImage[1]);
    pCmd->EndDebugUtilsLabel();
}

USING_RENDER_NAMESPACE_END
