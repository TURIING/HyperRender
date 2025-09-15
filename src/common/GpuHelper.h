/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/6/14 13:20
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GPUHELPER_H
#define GPUHELPER_H

#include "common.h"

USING_RENDER_NAMESPACE_BEGIN

class GpuHelper {
public:
    NODISCARD static HyperGpu::Buffer* CreateUniformBuffer(HyperGpu::GpuDevice* pDevice, uint32_t size);
    NODISCARD static HyperGpu::Image2D* CreateImage(HyperGpu::GpuDevice* pDevice, const Size& size, HyperGpu::Sampler* pSampler, const char* name = nullptr, HyperGpu::PixelFormat format = HyperGpu::PixelFormat::R8G8B8A8);
    static void CopyImage(HyperGpu::GpuDevice* pDevice, HyperGpu::GpuCmd* pCmd, HyperGpu::Image2D* pSrc, HyperGpu::Image2D* pDst, const Offset2D& srcOffset = {}, const Offset2D& dstOffset = {});
};

USING_RENDER_NAMESPACE_END

#endif //GPUHELPER_H
