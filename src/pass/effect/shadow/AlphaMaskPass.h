//
// Created by turiing on 2025/12/7.
//

#ifndef ALPHAMASKPASS_H
#define ALPHAMASKPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class AlphaMaskPass: public BasePass {
    struct LocalInfo {
        alignas(16) glm::vec3 shadowColor;
        alignas(4) float shadowAlpha;
    };
public:
    explicit AlphaMaskPass(HyperGpu::GpuDevice* pDevice);
    ~AlphaMaskPass() override;
    void SetLocalInfo(const Color &color);
    void SetTargetImage(HyperGpu::Image2D *pImage);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //ALPHAMASKPASS_H
