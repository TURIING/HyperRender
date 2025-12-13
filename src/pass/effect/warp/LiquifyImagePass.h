//
// Created by turiing on 2025/11/22.
//

#ifndef LIQUIFYIMAGEPASS_H
#define LIQUIFYIMAGEPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class LiquifyImagePass final : public BasePass {
public:
    explicit LiquifyImagePass(HyperGpu::GpuDevice* pGpuDevice);
    ~LiquifyImagePass() override;
    void SetUvInputImage(HyperGpu::Image2D* pImage);
    void SetTargetImage(HyperGpu::Image2D* pImage);
    void Draw(HyperGpu::GpuCmd *pCmd) override;
};

USING_RENDER_NAMESPACE_END

#endif //LIQUIFYIMAGEPASS_H
