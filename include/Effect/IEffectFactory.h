//
// Created by turiing on 2025/12/4.
//

#ifndef IEFFECTFACTORY_H
#define IEFFECTFACTORY_H

#include "../Render.h"
#include "blur/IDualKawaseBlur.h"
#include "warp/ILiquifyEffect.h"

namespace HyperRender {
    class IEffectFactory: public RenderObject {
    public:
        virtual IDualKawaseBlur* CreateDualKawaseBlurEffect() = 0;
        virtual ILiquifyEffect* CreateLiquifyEffect() = 0;
    };
}
#endif //IEFFECTFACTORY_H
