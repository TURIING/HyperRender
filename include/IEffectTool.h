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
#include "Effect/IEffect.h"
#include "Effect/IEffectFactory.h"

namespace HyperRender {
class IEffectTool : virtual public ITool {
public:
	virtual IEffectFactory* CreateEffectFactory() = 0;

	/**
	 * 设置施加效果的DrawUnit
	 * @param pTargetUnit
	 */
	virtual void SetTargetUnit(IDrawUnit* pTargetUnit) = 0;

	virtual void SetEffect(IEffect* pEffect) = 0;

	virtual void Begin(const Area &renderArea) = 0;

	virtual void DoRender() = 0;

	virtual void End() = 0;

	/**
	 * 将结果输出到某个DrawUnit
	 * @param resultUnit
	 */
	virtual void RenderToUnit(IDrawUnit* resultUnit) = 0;
};
}
#endif // IEFFECTTOOL_H
