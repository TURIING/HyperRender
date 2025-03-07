/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/5 21:41
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef TOOLFACTORY_H
#define TOOLFACTORY_H

#include "ITool.h"
#include "RenderType.h"
#include <memory>

namespace HyperGpu
{
    class GpuFactory;
    class GpuDevice;
}

namespace HyperRender
{
    struct PlatformSurfaceInfo {
        void *handle = nullptr;
        // Size size;
    };

    enum class ToolType { SCREEN_TOOL };

    class ToolFactory final {
    public:
        explicit ToolFactory(const PlatformSurfaceInfo &platformSurfaceInfo);
        ~ToolFactory();
        ITool *createTool(ToolType type);

    private:
        HyperGpu::GpuFactory *m_pGpuFactory = nullptr;
        std::unique_ptr<HyperGpu::GpuDevice> m_pGpuDevice = nullptr;
    };
}
#endif //TOOLFACTORY_H
