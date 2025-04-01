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

	m_pScreenPass->SubRef();
	m_pGpuDevice->SubRef();
}

void ScreenTool::Draw() {
	uint32_t imageIndex = 0;
	m_vecInFlightFence[m_currentFrameIndex]->Wait();
	m_pSurface->AcquireNextImage(m_vecImageAvailableSemaphore[m_currentFrameIndex], imageIndex);
	m_vecInFlightFence[m_currentFrameIndex]->Reset();

	GpuCmd::BeginRenderInfo beginRenderInfo{.pipeline			  = m_pScreenPass->GetPipeline(),
											.viewport			  = {0, 0, 1000, 1000},
											.scissor			  = {0, 0, 1000, 1000},
											.clearValue			  = {ClearValue{.color = {0.0f, 0.0f, 0.0f, 1.0f}}},
											.renderArea			  = {{0, 0}, {1000, 1000}},
											.renderAttachmentType = GpuCmd::RenderAttachmentType::Surface,
											.surface			  = m_pSurface};
	GpuCmd::DrawInfo		drawInfo{.vertexBuffer = m_pScreenPass->GetVertexBuffer(), .vertexCount = 3};
	m_vecCmd[m_currentFrameIndex]->Begin(beginRenderInfo);
	m_vecCmd[m_currentFrameIndex]->Draw(drawInfo);
	m_vecCmd[m_currentFrameIndex]->End();

	m_pGpuDevice->Submit(m_vecCmd[m_currentFrameIndex], m_vecImageAvailableSemaphore[m_currentFrameIndex], m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_vecInFlightFence[m_currentFrameIndex]);
	m_pGpuDevice->Present(m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_pSurface, imageIndex);

	m_currentFrameIndex = (m_currentFrameIndex + 1) % 3;
}
