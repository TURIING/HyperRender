/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/4/1 22:33
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef EFFECTTOOL_H
#define EFFECTTOOL_H

#include "BaseTool.h"
#include "IEffectTool.h"
#include "HyperGpu.h"

USING_RENDER_NAMESPACE_BEGIN

class RoundCornerPass;
class GaussianBlurPass;
class StrokePass;

class EffectTool final : public IEffectTool, public BaseTool {
public:
	explicit EffectTool(HyperGpu::GpuDevice* pGpuDevice);
	~EffectTool() override;
	IEffectFactory * CreateEffectFactory() override;
	void SetTargetUnit(IDrawUnit *pTargetUnit) override;
	void SetEffect(IEffect *pEffect) override;
	void Begin(const Area &renderArea) override;
	void DoRender() override;
	void End() override;
	void RenderToUnit(IDrawUnit *resultUnit) override;

private:
	RoundCornerPass* m_pRoundCornerPass = nullptr;
	GaussianBlurPass* m_pGaussianBlurPass = nullptr;
	StrokePass* m_pStrokePass = nullptr;
	DrawUnit* m_pTargetUnit = nullptr;
	DrawUnit* m_pResultUnit = nullptr;
	IEffect* m_pEffect = nullptr;
};

USING_RENDER_NAMESPACE_END
#endif // EFFECTTOOL_H
