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
class IEffectTool : public ITool {
public:
	struct BeginInfo {
		IDrawUnit* targetUnit = nullptr;	// 施加效果的DrawUnit
		Area renderArea;					// 影响区域
	};

public:
	virtual void Begin(const BeginInfo& info) = 0;

	/**
	 * @param radius 圆角度数，值在0-1之间
	 */
	virtual void SetRoundCorner(float radius) = 0;

	/**
	 * @param resultUnit 将结果输出到某个DrawUnit，也可以为空，为空即在targetUnit里绘制
	 */
	virtual void End(IDrawUnit* resultUnit = nullptr) = 0;
};
}
#endif // IEFFECTTOOL_H
