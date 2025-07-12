/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 14:39
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ROUNDCORNERPASS_H
#define ROUNDCORNERPASS_H

#include "../../common/common.h"
#include "../BasePass.h"
#include "HyperGpu.h"

USING_RENDER_NAMESPACE_BEGIN

class RoundCornerPass final : public BasePass {
    struct Vertex {
        glm::vec2 aPos;
    };

    struct LocalInfo {
        alignas(16) glm::vec2 resolution;
    };

public:
    explicit RoundCornerPass(HyperGpu::GpuDevice* pGpuDevice);
    ~RoundCornerPass() override;
    void SetMainTexture(HyperGpu::Image2D* mainTexture);

private:
    HyperGpu::Buffer*   m_pLocalBuffer  = nullptr;
    LocalInfo           m_localInfo{};
};

USING_RENDER_NAMESPACE_END

#endif //ROUNDCORNERPASS_H
