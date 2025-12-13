//
// Created by turiing on 2025/12/13.
//

#ifndef ISWIRLWARPEFFECT_H
#define ISWIRLWARPEFFECT_H
#include "../../RenderType.h"
#include "../IEffect.h"

namespace HyperRender {
    class ISwirlWarpEffect : virtual public IEffect {
    public:
        /**
         *
         * @param center 旋转中心坐标
         * @param angle 旋转角度，角度制
         * @param radius 旋转半径
         */
        virtual void SetSwirlInfo(const PointI &center, float angle, float radius) = 0;
    };
}

#endif //ISWIRLWARPEFFECT_H
