/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/1 22:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "EffectTool.h"
#include "../pass/effect/RoundCornerPass.h"

USING_RENDER_NAMESPACE_BEGIN

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {
    m_pRoundCornerPass = new RoundCornerPass(m_pGpuDevice);
}

EffectTool::~EffectTool() {
    m_pRoundCornerPass->SubRef();
}

void EffectTool::SetRoundCorner(float radius) {
}

void EffectTool::Begin(const Area &renderArea) {
    m_renderArea = renderArea;

    if (m_pResultUnit && renderArea.size == m_pResultUnit->GetTextureSize()) {
        m_pResultUnit->SetArea(renderArea);
    }
    else {
        if (m_pResultUnit) m_pResultUnit->SubRef();
        m_pResultUnit = BaseTool::CreateDrawUnit(renderArea);
    }

    m_pTargetUnit->AddRef();
    begin();
}

void EffectTool::Render() {
    auto image = m_pResultUnit->GetImage();
    HyperGpu::BeginRenderInfo beginInfo {
        .pPipeline = m_pRoundCornerPass->GetPipeline(),
        .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
        .renderArea = std::bit_cast<HyperGpu::Area>(m_renderArea),
        .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
        .renderAttachment = {1, &image},
    };
    m_pCmd->BeginRenderPass(beginInfo);
    m_pCmd->SetViewport({0, 0, (float)m_renderArea.size.width, (float)m_renderArea.size.height});
    m_pCmd->SetScissor({0, 0, m_renderArea.size.width, m_renderArea.size.height});
    m_pRoundCornerPass->Draw(m_pCmd);
    m_pCmd->EndRenderPass();
}

void EffectTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = pTargetUnit;
}

void EffectTool::End() {
    end();
    if (m_pTargetUnit) m_pTargetUnit->SubRef();
}

void EffectTool::RenderToUnit(IDrawUnit *resultUnit) {
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END