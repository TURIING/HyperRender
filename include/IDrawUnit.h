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

class IDrawUnit : public RenderObject {
public:
	virtual ~IDrawUnit() = default;
};
#endif // IDRAWUNIT_H
