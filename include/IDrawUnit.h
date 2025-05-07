/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/4/1 22:35
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef IDRAWUNIT_H
#define IDRAWUNIT_H

#include "Render.h"
#include "RenderType.h"

namespace HyperRender {
    class IDrawUnit : public RenderObject {
    public:
        [[nodiscard]] virtual HyperRender::Area GetArea() const = 0;
    };

}
#endif // IDRAWUNIT_H
