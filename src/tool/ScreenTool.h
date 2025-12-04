/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 13:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENTOOL_H
#define SCREENTOOL_H

#include "BaseTool.h"
#include "../../include/IScreenTool.h"
#include "../common/common.h"


namespace HyperGpu {
class Sampler;
class Image2D;
class Semaphore;
class Fence;
class GpuCmd;
class GpuDevice;
class GpuSurface;
}

USING_RENDER_NAMESPACE_BEGIN
class ScreenWindowTarget;
class ScreenPass;

class ScreenTool final : public IScreenTool, public BaseTool {
public:
    explicit ScreenTool(HyperGpu::GpuDevice *gpuDevice);
    ~ScreenTool() override;
    IScreenTarget *CreateScreen(const HyperRender::PlatformWindowInfo &platformSurfaceInfo) override;
    void SetScreenTarget(IScreenTarget *target) override;
    void AddScreenObject(IDrawUnit *pObjUnit, const Transform& transform = {}) override;
    void Begin(const Area &updateArea) override;
    void DoRender() override;
    void End() override;

private:
    void renderToScreen();

private:
    ScreenPass *m_pScreenPass = nullptr;
    HyperGpu::GpuSurface *m_pSurface = nullptr;
    ScreenWindowTarget* m_pScreenTarget = nullptr;
    HyperGpu::Semaphore* m_pRenderSemaphore = nullptr;
    HyperGpu::Image2D* m_pScreenTexture = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif // SCREENTOOL_H
