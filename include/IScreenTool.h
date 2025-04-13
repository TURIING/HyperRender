/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/7 13:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ISCREENTOOL_H
#define ISCREENTOOL_H

#include "ITool.h"
#include "RenderType.h"

namespace HyperRender {
class IScreenTool : virtual public ITool {
public:
	struct BeginInfo {
		IDrawUnit* targetUnit = nullptr;
		Area       renderArea;
	};

public:
	virtual void Begin(const BeginInfo& beginInfo) = 0;
	virtual void Draw() = 0;
};
}
#endif //ISCREENTOOL_H
