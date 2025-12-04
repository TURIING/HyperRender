//
// Created by turiing on 2025/12/4.
//

#ifndef IDUALKAWASEBLUR_H
#define IDUALKAWASEBLUR_H

#include "../../Render.h"
#include "../../RenderType.h"
#include "../IEffect.h"

namespace HyperRender {
    class IDualKawaseBlur: virtual public IEffect {
    public:
        virtual void SetBlurInfo(int interation, const Offset2D &offset) = 0;
    };
}
#endif //IDUALKAWASEBLUR_H
