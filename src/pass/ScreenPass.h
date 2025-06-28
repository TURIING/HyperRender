/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:19
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENPASS_H
#define SCREENPASS_H

#include "../common/common.h"
#include "BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class ScreenPass : public BasePass {
public:
    explicit ScreenPass(HyperGpu::GpuDevice* gpuDevice);
	~ScreenPass() override;
    void SetScreenTexture(HyperGpu::Image2D* screenTexture);
};

USING_RENDER_NAMESPACE_END

#endif //SCREENPASS_H