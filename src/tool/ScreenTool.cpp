/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/7 13:26
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "ScreenTool.h"
#include "ScreenTarget/ScreenWindowTarget.h"
#include <GpuDevice.h>
#include "../common/GpuHelper.h"
#include "../pass/ScreenPass.h"

USING_RENDER_NAMESPACE_BEGIN

ScreenTool::ScreenTool(HyperGpu::GpuDevice* gpuDevice) : BaseTool(gpuDevice) {
    m_pRenderSemaphore = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
    m_pScreenPass = new ScreenPass(gpuDevice);
}

ScreenTool::~ScreenTool() {
    m_pRenderSemaphore->SubRef();
    m_pScreenTexture->SubRef();
    m_pScreenTarget->SubRef();
    m_pScreenPass->SubRef();
}

IScreenTarget* ScreenTool::CreateScreen(const PlatformWindowInfo& platformSurfaceInfo) {
    return new ScreenWindowTarget(m_pGpuDevice, platformSurfaceInfo);
}

void ScreenTool::SetScreenTarget(IScreenTarget* target) {
    m_pScreenTarget = dynamic_cast<ScreenWindowTarget*>(target);
    m_pScreenTarget->AddRef();
}

void ScreenTool::AddScreenObject(IDrawUnit *pObjUnit, const Transform& transform) {
    const auto unit = dynamic_cast<DrawUnit*>(pObjUnit);
    m_pScreenPass->AddScreenTexture(unit->GetImage(), unit->GetArea().offset, transform);
}

void ScreenTool::Begin(const Area& updateArea) {
    m_renderArea = updateArea;
    updateSize(updateArea.size);
    m_pScreenPass->SetScreenSize(updateArea.size);
    // m_pScreenPass->SetNeedAliasing(true);

    if (!m_pScreenTexture) {
        m_pScreenTexture = GpuHelper::CreateImage(
            m_pGpuDevice,
            updateArea.size,
            m_pCommonSampler,
            "ScreenTexture",
            HyperGpu::PixelFormat::R8G8B8A8_SRGB,
            HyperGpu::SampleCountFlags::SAMPLE_COUNT_1_BIT
        );
    }

    begin();
}

void ScreenTool::DoRender() {
    BEGIN_CMD_DEBUG_LABEL(m_pCmd, "ScreenTool::DoRender");
    m_pScreenPass->SetBlendType(BlendType::Normal);
    m_pScreenPass->SetGlobalUniform(m_pGlobalBuffer);
    HyperGpu::BeginRenderInfo beginInfo {
        .pPipeline = m_pScreenPass->GetPipeline(),
        .clearValue = {{ .color = {1.0, 1.0, 1.0, 1}}},
        .renderArea = std::bit_cast<HyperGpu::Area>(m_renderArea),
        .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
        .renderAttachment = {1, (HyperGpu::Image2D*[])m_pScreenTexture},
    };
    m_pCmd->BeginRenderPass(beginInfo);
    m_pCmd->SetViewport({0, 0, (float)m_renderArea.size.width, (float)m_renderArea.size.height});
    m_pCmd->SetScissor({0, 0, m_renderArea.size.width, m_renderArea.size.height});
    m_pScreenPass->Draw(m_pCmd);
    m_pCmd->EndRenderPass();
    END_CMD_DEBUG_LABEL(m_pCmd, "ScreenTool::DoRender");
}

void ScreenTool::End() {
    end({m_pRenderSemaphore});
    renderToScreen();
}

void ScreenTool::renderToScreen() {
    ScreenWindowTarget::DrawToTargetInfo info{
        .pPresentSemaphore = &m_pRenderSemaphore,
        .semaphoreCount = 1,
        .pImage = m_pScreenTexture,
        .srcArea = m_renderArea,
        .dstArea = m_renderArea,
    };
    m_pScreenTarget->RenderToScreen(info);
}

USING_RENDER_NAMESPACE_END
