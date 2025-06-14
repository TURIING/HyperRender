/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/6/14 13:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "GpuHelper.h"

HyperGpu::Buffer* HyperRender::GpuHelper::CreateUniformBuffer(HyperGpu::GpuDevice* pDevice, uint32_t size) {
    return pDevice->GetResourceManager()->CreateBuffer({
        .bufferType = HyperGpu::Buffer::Uniform,
        .bufferSize = size,
    });
}
