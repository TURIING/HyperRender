//
// Created by turiing on 2025/12/4.
//

#ifndef ILIQUIFYEFFECT_H
#define ILIQUIFYEFFECT_H

#include "../../Render.h"
#include "../../RenderType.h"
#include "../IEffect.h"

namespace HyperRender {
    class ILiquifyEffect : virtual public IEffect {
    public:
        virtual void SetLiquifyInfo(const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) = 0;
    };
}
#endif //ILIQUIFYEFFECT_H
