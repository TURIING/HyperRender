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

class ScreenPass;

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

class ScreenTool final : public IScreenTool, public BaseTool {
public:
    explicit ScreenTool(HyperGpu::GpuDevice *gpuDevice);
    ~ ScreenTool() override;
    IScreenTarget *CreateScreen(const HyperRender::PlatformWindowInfo &platformSurfaceInfo) override;

    void SetScreenTarget(IScreenTarget *target) override;
    void BeginRenderToScreen(const Area &updateArea) override;
    void EndRenderToScreen() override;

private:
    void renderToScreen();

private:
    HyperGpu::GpuSurface *m_pSurface = nullptr;
    ScreenWindowTarget* m_pScreenTarget = nullptr;
    HyperGpu::Semaphore* m_pRenderSemaphore = nullptr;
    DrawUnit* m_pScreenTexture = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif // SCREENTOOL_H
