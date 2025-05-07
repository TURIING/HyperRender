/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/5 21:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "../../include/ToolFactory.h"

#include "EffectTool.h"
#include "GpuDevice.h"
#include "ScreenTool.h"

using namespace HyperGpu;

HyperRender::ToolFactory::ToolFactory() {
	Singleton<LogManager>::GetInstance()->Init();

    m_pGpuFactory = new GpuFactory(GpuFactory::VULKAN);

	QueueInfo queueInfos[] = {
		{ QueueType::Graphics, 1.0 },
		{ QueueType::Present, 0.8 }
	};

	const DeviceCreateInfo deviceInfo{
		.pQueueInfo = queueInfos,
		.queueInfoCount = std::size(queueInfos)
	};
	m_pGpuDevice = m_pGpuFactory->CreateDevice(deviceInfo);
}

HyperRender::ToolFactory::~ToolFactory() {
	GpuFactory::DestroyDevice(m_pGpuDevice);
	m_pGpuFactory->SubRef();
}

HyperRender::IScreenTool* HyperRender::ToolFactory::CreateScreenTool() const {
	return new ScreenTool(m_pGpuDevice);
}

HyperRender::IEffectTool* HyperRender::ToolFactory::CreateEffectTool() const {
	return new EffectTool(m_pGpuDevice);
}
