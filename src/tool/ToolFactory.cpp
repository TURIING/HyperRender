/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/5 21:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "../../include/ToolFactory.h"
#include "../../HyperGpu/include/GpuDevice.h"

using namespace HyperGpu;

HyperRender::ToolFactory::ToolFactory(const PlatformSurfaceInfo &platformSurfaceInfo) {
    m_pGpuFactory = new GpuFactory(GpuFactory::VULKAN);
    // const DeviceCreateInfo deviceInfo {
    //     .platformWindowInfo = {
    //         .handle = platformSurfaceInfo.handle,
    //         // .size = { platformSurfaceInfo.size.width, platformSurfaceInfo.size.height },
    //     },
    // };
    // m_pGpuDevice = m_pGpuFactory->CreateDevice(deviceInfo);

}

HyperRender::ToolFactory::~ToolFactory() {
}

ITool* HyperRender::ToolFactory::createTool(ToolType type) {
}
