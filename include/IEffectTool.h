/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/4/1 22:33
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef IEFFECTTOOL_H
#define IEFFECTTOOL_H

#include "IDrawUnit.h"
#include "ITool.h"
#include "RenderType.h"

namespace HyperRender {
class IEffectTool : virtual public ITool {
public:
	virtual void Begin(const Area &renderArea) = 0;

	/**
	 * 设置施加效果的DrawUnit
	 * @param pTargetUnit
	 */
	virtual void SetTargetUnit(IDrawUnit* pTargetUnit) = 0;

	/**
	 * @param radius 圆角度数，值在0-1之间
	 */
	virtual void SetRoundCorner(float radius) = 0;

	/**
	 * DualKawaseBlur
	 * @param interation 迭代次数
	 * @param offset 偏移
	 */
	virtual void DoDualKawaseBlur(int interation, const Offset2D &offset) = 0;

	virtual void End() = 0;

	/**
	 * 将结果输出到某个DrawUnit
	 * @param resultUnit
	 */
	virtual void RenderToUnit(IDrawUnit* resultUnit) = 0;
};
}
#endif // IEFFECTTOOL_H
