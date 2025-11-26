//
// Created by turiing on 2025/11/24.
//

#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "../common/common.h"
#include "BaseTool.h"
#include "ISelectionTool.h"
#include "HyperGpu.h"

USING_RENDER_NAMESPACE_BEGIN
class SelectionNormalPass;

class SelectionTool final: public ISelectionTool, public BaseTool {
public:
    explicit SelectionTool(HyperGpu::GpuDevice* pGpuDevice);
    ~SelectionTool() override;
    void Begin(const Area &renderArea) override;
    void SetTargetUnit(IDrawUnit *pTargetUnit) override;
    void DoNormalSelection(const Area &area, const Color &color, float density, float time) override;
    void End() override;
    void RenderToUnit(IDrawUnit *resultUnit) override;
private:
    SelectionNormalPass* m_pSelectionNormalPass = nullptr;
    DrawUnit* m_pTargetUnit = nullptr;
    DrawUnit* m_pResultUnit = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //SELECTIONTOOL_H
