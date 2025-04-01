/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 13:19
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef SCREENPASS_H
#define SCREENPASS_H

#include "../common/common.h"
#include "IPass.h"

namespace HyperGpu
{
    class Pipeline;
    class GpuDevice;
    class Buffer;
}

using namespace HyperGpu;

class ScreenPass: public IPass{
public:
    explicit ScreenPass(GpuDevice* gpuDevice);
	~ScreenPass() override;
    [[nodiscard]] Pipeline* GetPipeline() const { return m_pPipeline; }
	[[nodiscard]] Buffer* GetVertexBuffer() const { return m_pVertexBuffer; }

private:
    GpuDevice* m_pGpuDevice = nullptr;
    Pipeline* m_pPipeline = nullptr;
    Buffer* m_pVertexBuffer = nullptr;
    std::vector<float> m_vertexData = {
         0.0f, -0.5f, 0.0f,
         0.5f, 0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f
    };
};



#endif //SCREENPASS_H
