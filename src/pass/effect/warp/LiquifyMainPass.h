//
// Created by turiing on 2025/11/22.
//

#ifndef LIQUIFYMAINPASS_H
#define LIQUIFYMAINPASS_H

#include "../../../common/common.h"
#include "../../BasePass.h"

USING_RENDER_NAMESPACE_BEGIN
class LiquifyMainPass final : public BasePass {
    struct LocalInfo {
        alignas(8) glm::vec2 resolution;
        alignas(8) glm::vec2 newMouse;
        alignas(8) glm::vec2 oldMouse;
        alignas(4) int firstFrame;
        alignas(4) int isPressed;
    };
public:
    explicit LiquifyMainPass(HyperGpu::GpuDevice* pGpuDevice);
    ~LiquifyMainPass() override;
    void SetUvInputImage(HyperGpu::Image2D* pImage);
    void SetLocalInfo(const Size &resolution, const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame);
    void Draw(HyperGpu::GpuCmd *pCmd) override;

private:
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //LIQUIFYMAINPASS_H
