//
// Created by turiing on 2025/11/26.
//

#ifndef IACTIONFACTORY_H
#define IACTIONFACTORY_H

#include "IAction.h"
#include "../RenderType.h"

namespace HyperRender {
class IActionFactory {
public:
    virtual ~IActionFactory() = default;

    virtual void DeleteAction(IAction* pAction) = 0;

    /**
     * 颜色转换
     * @param color 欲改变成的颜色
     * @return 颜色转换action对象
     */
    virtual IAction* ColorChange(const Color& color) = 0;
};
}
#endif //IACTIONFACTORY_H
