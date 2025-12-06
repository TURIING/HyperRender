//
// Created by turiing on 2025/12/5.
//

#ifndef IEMBOSSFILTER_H
#define IEMBOSSFILTER_H

#include "../IEffect.h"

namespace HyperRender {
    class IEmbossFilter: virtual public IEffect {
    public:
        /**
         * 设置浮雕信息
         * @param azimuth 方位角, 控制光从左上、左下、右上、右下照射，单位角度，0度指向右边，逆时针递增
         * @param elevation 俯角, 控制光的高度，光线与地平面的夹角，单位角度
         * @param bumpHeight 控制凹凸程度，一般 0 开始
         * @param useGray 是否使用灰度图表示
         */
        virtual void SetEmbossInfo(float azimuth, float elevation, float bumpHeight, bool useGray = false) = 0;
    };
}
#endif //IEMBOSSFILTER_H
