//
// Created by turiing on 2025/12/5.
//

#ifndef EMBOSSFILTER_H
#define EMBOSSFILTER_H

#include "../../../common/common.h"
#include "../BaseEffect.h"
#include "Effect/filter/IEmbossFilter.h"

USING_RENDER_NAMESPACE_BEGIN
class EmbossFilterPass;

class EmbossFilter: public BaseEffect, public IEmbossFilter {
public:
    explicit EmbossFilter(HyperGpu::GpuDevice* pDevice);
    ~EmbossFilter() override;
    void SetEmbossInfo(float azimuth, float elevation, float bumpHeight, bool useGray) override;
    void DoRender(HyperGpu::GpuCmd *pCmd) override;

private:
    EmbossFilterPass* m_pEmbossFilterPass = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //EMBOSSFILTER_H
