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
    // m_pRoundCornerPass->SubRef();
    // m_pImageAvailableSemaphore->SubRef();
    // m_pRenderFinishedSemaphore->SubRef();
    // m_pInFlightFence->SubRef();
    // m_pCmd->SubRef();
}

void EffectTool::Begin(const BeginInfo& info) {
    // this->beginRender();
    m_pRoundCornerPass->SetScreenTexture(info.targetUnit);
    m_renderArea = info.renderArea;
}

void EffectTool::SetRoundCorner(float radius) {
}

void EffectTool::End(IDrawUnit* resultUnit) {
    // m_pRoundCornerPass->UpdateResource();
    //
    // auto       unit     = dynamic_cast<DrawUnit*>(resultUnit)->GetImage();
    // const auto viewport = HyperGpu::Viewport{
    //     .x = static_cast<float>(m_renderArea.offset.x),
    //     .y = static_cast<float>(m_renderArea.offset.y),
    //     .width = static_cast<float>(m_renderArea.size.width),
    //     .height = static_cast<float>(m_renderArea.size.height),
    // };
    //
    // HyperGpu::GpuCmd::BeginRenderInfo beginRenderInfo{
    //     .pipeline = m_pRoundCornerPass->GetPipeline(),
    //     .viewport = viewport,
    //     .scissor = std::bit_cast<HyperGpu::Scissor>(m_renderArea),
    //     .clearValue = {HyperGpu::ClearValue{.color = {0.0f, 0.0f, 0.0f, 0.0f}}},
    //     .renderArea = std::bit_cast<HyperGpu::Area>(m_renderArea),
    //     .renderAttachmentType = HyperGpu::GpuCmd::RenderAttachmentType::Image2D,
    //     .renderAttachment = {1, &unit}
    // };
    // const HyperGpu::GpuCmd::DrawInfo drawInfo{
    //     .inputAssembler = m_pRoundCornerPass->GetInputAssembler()
    // };
    //
    // m_pCmd->Begin(beginRenderInfo);
    // m_pCmd->Draw(drawInfo);
    // m_pCmd->End();
    //
    // endRender();
}

USING_RENDER_NAMESPACE_END