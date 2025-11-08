/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/19 20:51
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ISCREENTARGET_H
#define ISCREENTARGET_H

#include "Render.h"

namespace HyperRender
{
	struct PlatformWindowInfo {
		void* handle = nullptr;
		Size  size;
	};

	class IScreenTarget : public RenderObject {};
}

#endif //ISCREENTARGET_H
