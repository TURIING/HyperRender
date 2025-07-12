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
        [[nodiscard]] virtual Area GetArea() const = 0;
        [[nodiscard]] virtual Size GetSize() const = 0;
        [[nodiscard]] virtual Size GetTextureSize() const = 0;
        virtual void SetArea(const Area &newArea) = 0;
    };

}
#endif // IDRAWUNIT_H
