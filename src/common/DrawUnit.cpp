/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 17:46
* @version: 1.0
* @description: 
********************************************************************************/
#include "DrawUnit.h"
USING_RENDER_NAMESPACE_BEGIN

DrawUnit::DrawUnit(HyperGpu::GpuDevice* pGpuDevice, const DrawUnitCreateInfo &info): m_area(info.area), m_textureSize(info.area.size) {
    HyperGpu::Image2D::Image2DCreateInfo createInfo {
        .size = std::bit_cast<HyperGpu::Size>(info.area.size),
        .pSampler = info.pSampler,
        .format = HyperGpu::PixelFormat::R8G8B8A8,
        .usage = HyperGpu::Image2D::ImageUsage::Color,
        .objName = info.pName
    };
    m_pImage2D = pGpuDevice->GetResourceManager()->CreateImage2D(createInfo);
}

DrawUnit::~DrawUnit() {
    m_pImage2D->SubRef();
}

void DrawUnit::SetArea(const Area &newArea) {
    LOG_ASSERT_INFO(m_textureSize >= newArea.size, "Area too big");
    m_area = newArea;
}

USING_RENDER_NAMESPACE_END
