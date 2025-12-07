//
// Created by turiing on 2025/12/7.
//

#ifndef ISHADOWEFFECT_H
#define ISHADOWEFFECT_H

#include "IEffect.h"

namespace HyperRender {
    class IShadowEffect : virtual public IEffect {
    public:
        /**
         *
         * @param color 阴影颜色
         * @param offset 阴影偏移，单位px
         * @param soft 阴影柔和程度，从0开始，越大越柔和
         */
        virtual void SetShadowInfo(const Color &color, const Offset2D& offset, float soft) = 0;
    };
}
#endif //ISHADOWEFFECT_H
