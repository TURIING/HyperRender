/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:29
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef BASETOOL_H
#define BASETOOL_H

#include "../../include/ITool.h"
#include "../common/common.h"
#include "HyperGpu.h"

class BaseTool : virtual public HyperRender::ITool {
public:
	explicit BaseTool(HyperGpu::GpuDevice* pGpuDevice);
	~BaseTool() override;
	void         ClearColor(IDrawUnit* targetUnit, HyperRender::Color color) override;
	IDrawUnit*   CreateDrawUnit(const HyperRender::Area& area) override;

protected:
	HyperGpu::GpuCmd*    m_pCmd           = nullptr;
	HyperGpu::Sampler*   m_pCommonSampler = nullptr;
	HyperGpu::GpuDevice* m_pGpuDevice     = nullptr;

protected:
	HyperRender::Area m_renderArea;
};

#endif //BASETOOL_H
