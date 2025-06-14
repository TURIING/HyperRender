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
};

USING_RENDER_NAMESPACE_END

#endif //GPUHELPER_H
