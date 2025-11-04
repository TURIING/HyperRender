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

USING_RENDER_NAMESPACE_BEGIN

Color Color::White = { 1.0, 1.0, 1.0, 1.0};
Color Color::Black= { 0.0, 0.0, 0.0, 1.0};
Color Color::Red = { 1.0, 0.0, 0.0, 1.0};
Color Color::Green = { 0.0, 1.0, 0.0, 1.0};

ToolFactory::ToolFactory(GpuType type) {
	Singleton<LogManager>::GetInstance()->Init();

    m_pGpuFactory = new HyperGpu::GpuFactory(static_cast<HyperGpu::GpuFactory::GpuType>(type));

	HyperGpu::QueueInfo queueInfos[] = {
		{ HyperGpu::QueueType::Graphics, 1.0 },
		{ HyperGpu::QueueType::Present, 0.8 }
	};

	const HyperGpu::DeviceCreateInfo deviceInfo{
		.pQueueInfo = queueInfos,
		.queueInfoCount = std::size(queueInfos)
	};
	m_pGpuDevice = m_pGpuFactory->CreateDevice(deviceInfo);
}

ToolFactory::~ToolFactory() {
	HyperGpu::GpuFactory::DestroyDevice(m_pGpuDevice);
	m_pGpuFactory->SubRef();
}

IScreenTool* ToolFactory::CreateScreenTool() const {
	return new ScreenTool(m_pGpuDevice);
}

IEffectTool* ToolFactory::CreateEffectTool() const {
	return new EffectTool(m_pGpuDevice);
}

USING_RENDER_NAMESPACE_END