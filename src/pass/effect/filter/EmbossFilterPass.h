//
// Created by turiing on 2025/12/5.
//

#ifndef EMBOSSFILTERPASS_H
#define EMBOSSFILTERPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class EmbossFilterPass: public BasePass {
    struct LocalInfo {
        alignas(4) float azimuth;
        alignas(4) float elevation;
        alignas(4) float bumpHeight;
        alignas(4) int useGray;
    };
public:
    explicit EmbossFilterPass(HyperGpu::GpuDevice* pDevice);
    ~EmbossFilterPass() override;
    void SetTargetImage(HyperGpu::Image2D *pImage);
    void SetLocalInfo(float azimuth, float elevation, float bumpHeight, bool useGray);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //EMBOSSFILTERPASS_H
