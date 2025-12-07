//
// Created by turiing on 2025/9/13.
//

#ifndef GAUSSIANBLURPASS_H
#define GAUSSIANBLURPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class GaussianBlurPass final: public BasePass {
    struct LocalInfo {
        alignas(4) float radius;         // blur radius in pixels (>=0)
        alignas(4) int direction;       // 0 horizontal, 1 vertical
    };
public:
    explicit GaussianBlurPass(HyperGpu::GpuDevice* gpuDevice);
    ~GaussianBlurPass() override;
    void SetTargetImage(HyperGpu::Image2D* pImage);
    void SetHorizontalLocalInfo(float radius, bool direction);
    void SetVerticalLocalInfo(float radius, bool direction);

private:
    HyperGpu::Buffer* m_pLocalInfoHorizontalBuffer = nullptr;
    HyperGpu::Buffer* m_pLocalInfoVerticalBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //GAUSSIANBLURPASS_H
