//
// Created by turiing on 2025/12/9.
//

#ifndef IEXPOSUREFILTER_H
#define IEXPOSUREFILTER_H

#include "../IEffect.h"

namespace HyperRender {
    class IExposureFilter: virtual public IEffect {
    public:
        /**
         * 设置浮雕信息
         * @param exposure 曝光度
         */
        virtual void SetExposureInfo(float exposure) = 0;
    };
}

#endif //IEXPOSUREFILTER_H
