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
class EffectTool final : public IEffectTool, public BaseTool {
public:
	explicit EffectTool(HyperGpu::GpuDevice* pGpuDevice);
	~EffectTool() override;
	void SetRoundCorner(float radius) override;
	void SetTargetUnit(IDrawUnit *pTargetUnit) override;
	void Begin(const Area &renderArea) override;
	void Render() override;
	void End() override;
	void RenderToUnit(IDrawUnit *resultUnit) override;

private:
	RoundCornerPass* m_pRoundCornerPass = nullptr;
	IDrawUnit* m_pTargetUnit = nullptr;
	DrawUnit* m_pResultUnit = nullptr;
};

USING_RENDER_NAMESPACE_END
#endif // EFFECTTOOL_H
