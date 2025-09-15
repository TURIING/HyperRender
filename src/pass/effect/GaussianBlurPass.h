//
// Created by turiing on 2025/9/13.
//

#ifndef GAUSSIANBLURPASS_H
#define GAUSSIANBLURPASS_H

#include "../../common/common.h"
#include "../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class GaussianBlurPass final: public BasePass {
public:
    explicit GaussianBlurPass(HyperGpu::GpuDevice* gpuDevice);
    ~GaussianBlurPass() override;
    void SetTargetImage(HyperGpu::Image2D* pImage);
    void SetOutputImage(HyperGpu::Image2D* pImage);
};

USING_RENDER_NAMESPACE_END

#endif //GAUSSIANBLURPASS_H
