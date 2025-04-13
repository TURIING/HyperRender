/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:29
* @version: 1.0
* @description: 
********************************************************************************/
#include "BaseTool.h"
#include "../common/Camera.h"

BaseTool::BaseTool(HyperGpu::GpuDevice* pGpuDevice) : m_pGpuDevice(pGpuDevice) {
	m_pGpuDevice->AddRef();
	m_pCmd           = m_pGpuDevice->GetCmdManager()->CreateCommandBuffer();
	m_pCommonSampler = m_pGpuDevice->GetResourceManager()->CreateSampler({});
}

BaseTool::~BaseTool() {
	m_pCommonSampler->SubRef();
	m_pGpuDevice->SubRef();
}

void BaseTool::ClearColor(IDrawUnit* targetUnit, HyperRender::Color color) {
	const auto unit = dynamic_cast<DrawUnit*>(targetUnit);

	m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
		pCmd->ClearColorImage(unit->GetImage(), std::bit_cast<HyperGpu::Color>(color));
	});
}

IDrawUnit* BaseTool::CreateDrawUnit(const HyperRender::Area& area) {
	return new DrawUnit(m_pGpuDevice, {area, m_pCommonSampler});
}
