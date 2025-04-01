/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 13:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENTOOL_H
#define SCREENTOOL_H

#include "../../include/IScreenTool.h"
#include "../common/common.h"

class ScreenPass;

namespace HyperGpu {
class Semaphore;
class Fence;
class GpuCmd;
class GpuDevice;
class GpuSurface;
}

using namespace HyperGpu;
using namespace HyperRender;

class ScreenTool final : public IScreenTool {
public:
	explicit ScreenTool(GpuDevice* gpuDevice);
	~		 ScreenTool() override;
	void	 Draw() override;

private:
	GpuDevice*				m_pGpuDevice  = nullptr;
	GpuSurface*				m_pSurface	  = nullptr;
	ScreenPass*				m_pScreenPass = nullptr;
	std::vector<GpuCmd*>	m_vecCmd{3};
	std::vector<Semaphore*> m_vecImageAvailableSemaphore{3};
	std::vector<Semaphore*> m_vecRenderFinishedSemaphore{3};
	std::vector<Fence*>		m_vecInFlightFence{3};
	uint32_t				m_currentFrameIndex = 0;
};

#endif // SCREENTOOL_H
