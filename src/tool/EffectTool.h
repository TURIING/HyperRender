/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/4/1 22:33
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef EFFECTTOOL_H
#define EFFECTTOOL_H

#include "IEffectTool.h"

class EffectTool final : public HyperRender::IEffectTool {
public:
	void Begin(IDrawUnit* targetUnit, const HyperRender::Area& area) override;
	void SetRoundCorner(float radius) override;
	void End(IDrawUnit* resultUnit) override;
};

#endif // EFFECTTOOL_H
