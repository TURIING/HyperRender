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
	void     Begin(const BeginInfo& info) override;
	void     SetRoundCorner(float radius) override;
	void     End(IDrawUnit* resultUnit) override;

private:
	RoundCornerPass* m_pRoundCornerPass = nullptr;
};

USING_RENDER_NAMESPACE_END
#endif // EFFECTTOOL_H
