//
// Created by turiing on 2025/11/22.
//

#ifndef LIQUIFYPASS_H
#define LIQUIFYPASS_H

#include "../../common/common.h"
#include "../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class LiquifyPass final : public BasePass {
    struct LocalInfo {
        alignas(8) glm::vec2 resolution;
        alignas(8) glm::vec2 newMouse;
        alignas(8) glm::vec2 oldMouse;
        alignas(4) int firstFrame;
        alignas(4) int isPressed;
    };
public:
    explicit LiquifyPass(HyperGpu::GpuDevice* pGpuDevice);
    ~LiquifyPass() override;
    void SetTargetImage(HyperGpu::Image2D* pImage);
    void SetUvInputImage(HyperGpu::Image2D* pImage);
    void SetUvOutputImage(HyperGpu::Image2D* pImage);
    void SetOutputImage(HyperGpu::Image2D* pImage);
    void SetLocalInfo(const Size &resolution, const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame);

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};


USING_RENDER_NAMESPACE_END
#endif //LIQUIFYPASS_H
