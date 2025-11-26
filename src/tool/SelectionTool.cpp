//
// Created by turiing on 2025/11/24.
//

#include "SelectionTool.h"
#include "../pass/selection/SelectionNormalPass.h"

USING_RENDER_NAMESPACE_BEGIN
SelectionTool::SelectionTool(HyperGpu::GpuDevice *pGpuDevice): BaseTool(pGpuDevice) {
    m_pSelectionNormalPass = new SelectionNormalPass(pGpuDevice);
}

SelectionTool::~SelectionTool() {
    m_pSelectionNormalPass->SubRef();
}

void SelectionTool::Begin(const Area &renderArea) {
    m_renderArea = renderArea;
    begin();
    m_pCmd->BeginDebugUtilsLabel("SelectionTool::Begin");
}

void SelectionTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = dynamic_cast<DrawUnit *>(pTargetUnit);

    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    m_pResultUnit = BaseTool::CreateDrawUnit(m_pTargetUnit->GetArea(), "SelectionToolResultUnit");
}

void SelectionTool::DoNormalSelection(const Area &area, const Color &color, float density, float time) {
    const auto size = m_renderArea.size;
    const glm::vec2 rectMin = { area.offset.x / (float)size.width, area.offset.y / (float)size.height };
    const glm::vec2 rectMax = { (area.offset.x+area.size.width)/(float)size.width, (area.offset.y+area.size.height)/(float)size.height };
    m_pSelectionNormalPass->SetLocalInfo(rectMin, rectMax, std::bit_cast<glm::vec4>(color), density, time);

    HyperGpu::BeginRenderInfo beginInfo {
        .pPipeline = m_pSelectionNormalPass->GetPipeline(),
        .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
        .renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
        .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
        .renderAttachment = {1, (HyperGpu::Image2D*[]){m_pResultUnit->GetImage()}},
    };
    m_pCmd->BeginRenderPass(beginInfo);
    m_pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
    m_pCmd->SetScissor({0, 0, size.width, size.height});
    m_pSelectionNormalPass->Draw(m_pCmd);
    m_pCmd->EndRenderPass();
}

void SelectionTool::End() {
    m_pCmd->EndDebugUtilsLabel();
    end();
}

void SelectionTool::RenderToUnit(IDrawUnit *resultUnit) {
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END
