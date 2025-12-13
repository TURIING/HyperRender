//
// Created by turiing on 2025/12/9.
//

#ifndef EXPOSUREFILTERPASS_H
#define EXPOSUREFILTERPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class ExposureFilterPass: public BasePass {
    struct LocalInfo {
        alignas(4) float exposure = 0.0;
    };

public:
    explicit ExposureFilterPass(HyperGpu::GpuDevice* pDevice);
    ~ExposureFilterPass() override;
    void SetTargetImage(HyperGpu::Image2D *pImage);
    void SetLocalInfo(float exposure);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //EXPOSUREFILTERPASS_H
