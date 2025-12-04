/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/1 22:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "EffectTool.h"
#include "../common/GpuHelper.h"
#include "Effect/EffectFactory.h"
#include "Effect/BaseEffect.h"

USING_RENDER_NAMESPACE_BEGIN

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {}

EffectTool::~EffectTool() {
    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
}

IEffectFactory * EffectTool::CreateEffectFactory() {
	return new EffectFactory(m_pGpuDevice);
}

void EffectTool::Begin(const Area &renderArea) {
    m_renderArea = renderArea;
    begin();
	m_pCmd->BeginDebugUtilsLabel("EffectTool::Begin");
}

void EffectTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = dynamic_cast<DrawUnit *>(pTargetUnit);

    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    m_pResultUnit = BaseTool::CreateDrawUnit(m_pTargetUnit->GetArea(), "EffectToolResultUnit");
}

void EffectTool::SetEffect(IEffect *pEffect) {
	m_pEffect = pEffect;
}

void EffectTool::DoRender() {
    auto pEffect = dynamic_cast<BaseEffect *>(m_pEffect);
    pEffect->SetTargetUnit(m_pTargetUnit);
    pEffect->SetResultUnit(m_pResultUnit);
	pEffect->DoRender(m_pCmd);
}

void EffectTool::End() {
	m_pCmd->EndDebugUtilsLabel();
    end();
}

void EffectTool::RenderToUnit(IDrawUnit *resultUnit) {
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END