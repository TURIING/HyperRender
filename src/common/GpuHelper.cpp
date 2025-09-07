/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/6/14 13:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

HyperGpu::Buffer* HyperRender::GpuHelper::CreateUniformBuffer(HyperGpu::GpuDevice* pDevice, uint32_t size) {
    return pDevice->GetResourceManager()->CreateBuffer({
        .bufferType = HyperGpu::Buffer::Uniform,
        .bufferSize = size,
    });
}

HyperGpu::Image2D* GpuHelper::CreateImage(HyperGpu::GpuDevice *pDevice, const Size &size, HyperGpu::Sampler *pSampler, const char *name, HyperGpu::PixelFormat format) {
    HyperGpu::Image2D::Image2DCreateInfo createInfo = {
        .size = std::bit_cast<HyperGpu::Size>(size),
        .pSampler = pSampler,
        .objName = name,
        .format = format,
    };
    return pDevice->GetResourceManager()->CreateImage2D(createInfo);
}

void GpuHelper::CopyImage(HyperGpu::GpuDevice* pDevice, HyperGpu::Image2D *pSrc, HyperGpu::Image2D *pDst, const Offset2D &srcOffset, const Offset2D &dstOffset) {
    const auto srcSize = pSrc->GetSize();
    const HyperGpu::Area srcArea {
        std::bit_cast<HyperGpu::Offset2D>(srcOffset),
        std::bit_cast<HyperGpu::Size>(srcSize),
    };

    const HyperGpu::Area dstArea {
        std::bit_cast<HyperGpu::Offset2D>(dstOffset),
        std::bit_cast<HyperGpu::Size>(srcSize),
    };

    std::vector<HyperGpu::ImageCopyRange> ranges = {
        {
            .srcArea = srcArea,
            .dstArea = dstArea
        },
    };
    pDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
        pCmd->CopyImage(pSrc, pDst, ranges.data(), ranges.size());
    });
}

USING_RENDER_NAMESPACE_END
