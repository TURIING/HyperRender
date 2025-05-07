/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/19 20:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENWINDOWTARGET_H
#define SCREENWINDOWTARGET_H

#include "../../common/common.h"
#include "IScreenTarget.h"
#include "ScreenTargetBase.h"

USING_RENDER_NAMESPACE_BEGIN

class ScreenWindowTarget final: public ScreenTargetBase, virtual public HyperRender::IScreenTarget {
public:
    ScreenWindowTarget(HyperGpu::GpuDevice* pGpuDevice, const PlatformWindowInfo& info);
    ~ScreenWindowTarget() override;
    void RenderToScreen(const DrawToTargetInfo& info) override;

private:
};

USING_RENDER_NAMESPACE_END

#endif //SCREENWINDOWTARGET_H
