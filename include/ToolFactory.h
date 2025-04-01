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

struct PlatformSurfaceInfo {
	void* handle = nullptr;
	Size  size;
};

class ToolFactory final : public RenderObject {
public:
	explicit				   ToolFactory(const PlatformSurfaceInfo& platformSurfaceInfo);
	~						   ToolFactory();
	[[nodiscard]] IScreenTool* CreateScreenTool() const;

private:
	HyperGpu::GpuFactory* m_pGpuFactory = nullptr;
	HyperGpu::GpuDevice*  m_pGpuDevice	= nullptr;
};
}
#endif // TOOLFACTORY_H
