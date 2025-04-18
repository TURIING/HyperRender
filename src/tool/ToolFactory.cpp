/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/5 21:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "../../include/ToolFactory.h"

#include "GpuDevice.h"
#include "ScreenTool.h"

using namespace HyperGpu;

HyperRender::ToolFactory::ToolFactory(const PlatformSurfaceInfo &platformSurfaceInfo) {
	Singleton<LogManager>::GetInstance()->Init();

    m_pGpuFactory = new GpuFactory(GpuFactory::VULKAN);
	const DeviceCreateInfo deviceInfo{
		.platformWindowInfo =
			{
				.handle = platformSurfaceInfo.handle,
				.size	= {platformSurfaceInfo.size.width, platformSurfaceInfo.size.height},
			},
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
