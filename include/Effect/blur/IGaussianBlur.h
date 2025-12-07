//
// Created by turiing on 2025/12/6.
//

#ifndef IGAUSSIANBLUR_H
#define IGAUSSIANBLUR_H

#include "../IEffect.h"

namespace HyperRender {
    class IGaussianBlur : virtual public IEffect {
    public:
        /**
         * 设置模糊信息
         * @param radius 模糊半径
         */
        virtual void SetBlurInfo(float radius) = 0;
    };
}
#endif //IGAUSSIANBLUR_H
