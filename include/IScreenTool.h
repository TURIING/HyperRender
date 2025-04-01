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

namespace HyperRender {
class IScreenTool : public ITool {
public:
    virtual ~IScreenTool() = default;
	virtual void Draw()		   = 0;
};
}
#endif //ISCREENTOOL_H
