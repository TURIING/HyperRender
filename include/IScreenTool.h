/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 13:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ISCREENTOOL_H
#define ISCREENTOOL_H

#include "ITool.h"
#include "RenderType.h"
#include "IScreenTarget.h"

namespace HyperRender {

    class IScreenTool : virtual public ITool {
    public:
        struct BeginInfo {
            IDrawUnit *targetUnit = nullptr;
            Area renderArea;
        };

    public:
        virtual IScreenTarget *CreateScreen(const PlatformWindowInfo &platformSurfaceInfo) = 0;

        virtual void SetScreenTarget(IScreenTarget *target) = 0;

        /**
         * 添加上屏对象
         * @param pObjUnit
         * @param area 上屏的区域
         */
        virtual void AddScreenObject(IDrawUnit* pObjUnit, const Transform& transform = {}) = 0;

        virtual void Begin(const Area &updateArea) = 0;
        virtual void DoRender() = 0;
        virtual void End() = 0;
    };

}

#endif //ISCREENTOOL_H
