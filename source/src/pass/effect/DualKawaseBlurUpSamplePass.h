//
// Created by turiing on 2025/8/20.
//

#ifndef DUALKAWASEBLURUPSAMPLEPASS_H
#define DUALKAWASEBLURUPSAMPLEPASS_H

#include "../../common/common.h"
#include "../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class DualKawaseBlurUpSamplePass : public BasePass {
    struct LocalInfo {
        alignas(8) glm::vec2 iResolution;
        alignas(8) glm::vec2 offset;
    };

public:
    explicit DualKawaseBlurUpSamplePass(HyperGpu::GpuDevice* gpuDevice);
    ~DualKawaseBlurUpSamplePass() override;
    void SetTargetTexture(HyperGpu::Image2D* targetTexture);
    void Draw(HyperGpu::GpuCmd *pCmd, int order);
    void SetScreenSize(const Size &size);
    void SetOffset(const Offset2D &offset);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer0 = nullptr;
    HyperGpu::Buffer* m_pLocalInfoBuffer1 = nullptr;
    HyperGpu::Buffer* m_pLocalInfoBuffer2 = nullptr;
    LocalInfo m_localInfo {};
    bool m_isLocalInfoDirty = true;
};

USING_RENDER_NAMESPACE_END

#endif //DUALKAWASEBLURUPSAMPLEPASS_H
