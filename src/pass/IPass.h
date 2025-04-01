/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:21
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef IPASS_H
#define IPASS_H

#include "Render.h"

class IPass : public RenderObject {
public:
    virtual ~IPass() = default;

};
#endif //IPASS_H
