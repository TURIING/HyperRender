//
// Created by turiing on 2025/12/4.
//

#ifndef IEFFECTFACTORY_H
#define IEFFECTFACTORY_H

#include "../Render.h"
#include "blur/IDualKawaseBlur.h"
#include "blur/IGaussianBlur.h"
#include "warp/ILiquifyEffect.h"
#include "filter/IEmbossFilter.h"

namespace HyperRender {
    class IEffectFactory: public RenderObject {
    public:
        virtual IDualKawaseBlur* CreateDualKawaseBlurEffect() = 0;
        virtual ILiquifyEffect* CreateLiquifyEffect() = 0;
        virtual IEmbossFilter* CreateEmbossFilter() = 0;
        virtual IGaussianBlur* CreateGaussianBlur() = 0;
    };
}
#endif //IEFFECTFACTORY_H
