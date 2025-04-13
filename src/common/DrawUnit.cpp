/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 17:46
* @version: 1.0
* @description: 
********************************************************************************/
#include "DrawUnit.h"

DrawUnit::DrawUnit(HyperGpu::GpuDevice* pGpuDevice, const DrawUnitCreateInfo &info): m_area(info.area) {
    HyperGpu::Image2D::Image2DCreateInfo createInfo {
        .size = std::bit_cast<HyperGpu::Size>(info.area.size),
        .pSampler = info.pSampler,
        .usage = HyperGpu::Image2D::ImageUsage::Color,
        .format = HyperGpu::PixelFormat::R8G8B8A8
    };
    m_pImage2D = pGpuDevice->GetResourceManager()->CreateImage2D(createInfo);
}

DrawUnit::~DrawUnit() {
    m_pImage2D->SubRef();
}
