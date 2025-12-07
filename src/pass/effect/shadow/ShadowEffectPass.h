//
// Created by turiing on 2025/12/7.
//

#ifndef SHADOWEFFECTPASS_H
#define SHADOWEFFECTPASS_H

#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class ShadowEffectPass: public BasePass {
    struct LocalInfo {
        alignas(8) glm::vec2 offset;
    };
public:
    explicit ShadowEffectPass(HyperGpu::GpuDevice* pGpuDevice);
    ~ShadowEffectPass() override;
    void SetLocalInfo(const Offset2D &offset);
    void SetTargetImage(HyperGpu::Image2D* pImage);
    void SetAlphaMask(HyperGpu::Image2D* pImage);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SHADOWEFFECTPASS_H
