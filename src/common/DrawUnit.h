/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 17:46
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef DRAWUNIT_H
#define DRAWUNIT_H

#include "IDrawUnit.h"
#include "HyperGpu.h"
#include "common.h"

class DrawUnit final: public RenderObject, public IDrawUnit {
public:
    struct DrawUnitCreateInfo {
        HyperRender::Area area;
        HyperGpu::Sampler* pSampler = nullptr;
    };

public:
    DrawUnit(HyperGpu::GpuDevice* pGpuDevice, const DrawUnitCreateInfo& info);
    ~DrawUnit() override;
    [[nodiscard]] HyperRender::Area GetArea() const override { return m_area; }
    [[nodiscard]] HyperGpu::Image2D* GetImage() const { return m_pImage2D; }

private:
    HyperGpu::Image2D* m_pImage2D = nullptr;
    HyperRender::Area m_area;
};



#endif //DRAWUNIT_H
