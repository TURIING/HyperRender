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
#include "Render.h"
#include "RenderType.h"

namespace HyperGpu {
class GpuFactory;
class GpuDevice;
}

namespace HyperRender {
class IScreenTool;
class IEffectTool;

enum class GpuType { OPENGL, VULKAN };

class ToolFactory final : public RenderObject {
public:
	explicit ToolFactory(GpuType type);
	~ToolFactory() override;
	[[nodiscard]] IScreenTool* CreateScreenTool() const;
	[[nodiscard]] IEffectTool* CreateEffectTool() const;

private:
	HyperGpu::GpuFactory* m_pGpuFactory = nullptr;
	HyperGpu::GpuDevice*  m_pGpuDevice	= nullptr;
};
}
#endif // TOOLFACTORY_H
