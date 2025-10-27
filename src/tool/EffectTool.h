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
class DualKawaseBlurDownSamplePass;
class DualKawaseBlurUpSamplePass;
class GaussianBlurPass;
class StrokePass;

class EffectTool final : public IEffectTool, public BaseTool {
public:
	explicit EffectTool(HyperGpu::GpuDevice* pGpuDevice);
	~EffectTool() override;
	void SetRoundCorner(float radius) override;
	void DoDualKawaseBlur(int interation, const Offset2D &offset) override;
	void DoGaussianBlur();
	void DoStroke() override;
	void SetTargetUnit(IDrawUnit *pTargetUnit) override;
	void Begin(const Area &renderArea) override;
	void End() override;
	void RenderToUnit(IDrawUnit *resultUnit) override;

private:
	RoundCornerPass* m_pRoundCornerPass = nullptr;
	DualKawaseBlurUpSamplePass* m_pDualKawaseBlurUpSamplePass = nullptr;
	DualKawaseBlurDownSamplePass* m_pDualKawaseBlurDownSamplePass = nullptr;
	GaussianBlurPass* m_pGaussianBlurPass = nullptr;
	StrokePass* m_pStrokePass = nullptr;
	DrawUnit* m_pTargetUnit = nullptr;
	DrawUnit* m_pResultUnit = nullptr;
};

USING_RENDER_NAMESPACE_END
#endif // EFFECTTOOL_H
