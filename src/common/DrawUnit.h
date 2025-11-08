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

USING_RENDER_NAMESPACE_BEGIN

class DrawUnit final: public IDrawUnit {
public:
    struct DrawUnitCreateInfo {
        Area area;
        HyperGpu::Sampler* pSampler = nullptr;
        const char* pName = nullptr;
    };

public:
    DrawUnit(HyperGpu::GpuDevice* pGpuDevice, const DrawUnitCreateInfo& info);
    ~DrawUnit() override;
    NODISCARD Area GetArea() const override { return m_area; }
    NODISCARD Size GetSize() const override { return m_area.size; }
    NODISCARD HyperGpu::Image2D* GetImage() const { return m_pImage2D; }
    NODISCARD Size GetTextureSize() const override { return m_textureSize; }
    void SetArea(const Area &newArea) override;

private:
    HyperGpu::Image2D* m_pImage2D = nullptr;
    Area m_area;
    Size m_textureSize;
};

USING_RENDER_NAMESPACE_END

#endif //DRAWUNIT_H