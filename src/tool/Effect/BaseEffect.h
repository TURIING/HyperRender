//
// Created by turiing on 2025/12/4.
//

#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include "../../common/common.h"
#include "Effect/IEffect.h"
#include "../../common/DrawUnit.h"

USING_RENDER_NAMESPACE_BEGIN
class BaseEffect : virtual public IEffect {
public:
    explicit BaseEffect(HyperGpu::GpuDevice* pGpuDevice): m_pGpuDevice(pGpuDevice) {
        m_pGpuDevice->AddRef();
    }

    ~BaseEffect() override {
        m_pGpuDevice->SubRef();
    }

    virtual void SetTargetUnit(DrawUnit* pUnit) { m_pTargetUnit = pUnit; }
    virtual void SetResultUnit(DrawUnit* pUnit) { m_pResultUnit = pUnit; }
    virtual void DoRender(HyperGpu::GpuCmd* pCmd) = 0;

protected:
    HyperGpu::GpuDevice* m_pGpuDevice = nullptr;
    DrawUnit* m_pTargetUnit = nullptr;
    DrawUnit* m_pResultUnit = nullptr;
};

USING_RENDER_NAMESPACE_END
#endif //BASEEFFECT_H
