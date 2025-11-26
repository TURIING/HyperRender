//
// Created by turiing on 2025/11/24.
//

#ifndef ISELECTIONTOOL_H
#define ISELECTIONTOOL_H

#include "IDrawUnit.h"
#include "ITool.h"
#include "RenderType.h"

namespace HyperRender {
    class ISelectionTool : virtual public ITool {
    public:
        virtual void Begin(const Area &renderArea) = 0;

        /**
         * 设置施加效果的DrawUnit
         * @param pTargetUnit
         */
        virtual void SetTargetUnit(IDrawUnit* pTargetUnit) = 0;

        /**
         * 施加正常效果的蚂蚁线
         * @param area 蚂蚁线区域
         * @param color 蚂蚁线颜色
         * @param density 密度
         * @param time 递增的时间因子，用于做动画
         */
        virtual void DoNormalSelection(const Area &area, const Color &color = Color::Black, float density = 10, float time = 0) = 0;

        virtual void End() = 0;

        /**
         * 将结果输出到某个DrawUnit
         * @param resultUnit
         */
        virtual void RenderToUnit(IDrawUnit* resultUnit) = 0;
    };
}
#endif //ISELECTIONTOOL_H
