/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/7 13:26
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "ScreenTool.h"

#include <GpuDevice.h>

#include "../pass/ScreenPass.h"

ScreenTool::ScreenTool(GpuDevice* gpuDevice) : m_pGpuDevice(gpuDevice) {
	m_pGpuDevice->AddRef();
	m_pScreenPass = new ScreenPass(gpuDevice);
	m_pSurface	  = m_pGpuDevice->GetSurface(m_pScreenPass->GetPipeline());

	for(auto i = 0; i < 3; i++) {
		m_vecImageAvailableSemaphore[i] = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
		m_vecRenderFinishedSemaphore[i] = m_pGpuDevice->GetSyncManager()->CreateSemaphore();
		m_vecInFlightFence[i]			= m_pGpuDevice->GetSyncManager()->CreateFence();
		m_vecCmd[i]						= m_pGpuDevice->GetCmdManager()->CreateCommandBuffer();
	}

	m_pSampler = gpuDevice->GetResourceManager()->CreateSampler({});
}

ScreenTool::~ScreenTool() {
	for(auto semaphore : m_vecRenderFinishedSemaphore) {
		GpuSyncManager::DestroySemaphore(semaphore);
	}

	for(auto semaphore : m_vecImageAvailableSemaphore) {
		GpuSyncManager::DestroySemaphore(semaphore);
	}

	for(auto fence : m_vecInFlightFence) {
		GpuSyncManager::DestroyFence(fence);
	}

	m_pBgTex->SubRef();
	m_pSampler->SubRef();

	m_pScreenPass->SubRef();
	m_pGpuDevice->SubRef();
}

void ScreenTool::Begin(const BeginInfo& beginInfo) {
	m_renderArea = beginInfo.renderArea;

	Image2D::Image2DCreateInfo createInfo = {
		.size = {std::bit_cast<HyperGpu::Size>(m_renderArea.size)},
		.format = PixelFormat::R8G8B8A8,
		.usage = Image2D::ImageUsage::Color,
		.pSampler = m_pSampler
	};
	m_pBgTex = m_pGpuDevice->GetResourceManager()->CreateImage2D(createInfo);
	m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](GpuCmd* pCmd) {
		pCmd->ClearColorImage(m_pBgTex, Color{1.0, 0.0, 0.0, 1.0});
	});
	m_pScreenPass->SetScreenTexture(m_pBgTex);
}

void ScreenTool::Draw() {
	m_pScreenPass->UpdateResource();

	uint32_t imageIndex = 0;
	m_vecInFlightFence[m_currentFrameIndex]->Wait();
	m_pSurface->AcquireNextImage(m_vecImageAvailableSemaphore[m_currentFrameIndex], imageIndex);
	m_vecInFlightFence[m_currentFrameIndex]->Reset();

	const auto viewport = Viewport{
		.x = static_cast<float>(m_renderArea.offset.x),
		.y = static_cast<float>(m_renderArea.offset.y),
		.width = static_cast<float>(m_renderArea.size.width),
		.height = static_cast<float>(m_renderArea.size.height),
	};

	GpuCmd::BeginRenderInfo beginRenderInfo{.pipeline			  = m_pScreenPass->GetPipeline(),
		.viewport = viewport,
		.scissor = std::bit_cast<HyperGpu::Scissor>(m_renderArea),
		.clearValue = {ClearValue{.color = {0.0f, 0.0f, 0.0f, 1.0f}}},
		.renderArea = std::bit_cast<HyperGpu::Area>(m_renderArea),
		.renderAttachmentType = GpuCmd::RenderAttachmentType::Surface,
		.surface = m_pSurface
	};
	const GpuCmd::DrawInfo drawInfo{
		.inputAssembler = m_pScreenPass->GetInputAssembler()
	};

	m_vecCmd[m_currentFrameIndex]->Begin(beginRenderInfo);
	m_vecCmd[m_currentFrameIndex]->Draw(drawInfo);
	m_vecCmd[m_currentFrameIndex]->End();

	m_pGpuDevice->Submit(m_vecCmd[m_currentFrameIndex], m_vecImageAvailableSemaphore[m_currentFrameIndex], m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_vecInFlightFence[m_currentFrameIndex]);
	m_pGpuDevice->Present(m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_pSurface, imageIndex);

	m_currentFrameIndex = (m_currentFrameIndex + 1) % 3;
}
