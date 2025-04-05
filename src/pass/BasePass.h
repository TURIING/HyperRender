/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:21
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef BASE_PASS_H
#define BASE_PASS_H

#include "Render.h"
#include "GpuCmd.h"

class BasePass : public RenderObject {
public:
    virtual void              UpdateResource() = 0;
    HyperGpu::InputAssembler* GetInputAssembler() { return &m_inputAssembler; }

protected:
    HyperGpu::InputAssembler m_inputAssembler;
};
#endif //BASE_PASS_H
