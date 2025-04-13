/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/5 21:50
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ITOOL_H
#define ITOOL_H

#include "Render.h"
#include "IDrawUnit.h"
#include "RenderType.h"

namespace HyperRender {
    class ITool : public RenderObject {
    public:
        virtual void       ClearColor(IDrawUnit* targetUnit, Color color) = 0;
        virtual IDrawUnit* CreateDrawUnit(const Area& area) = 0;
    };
}
#endif //ITOOL_H
