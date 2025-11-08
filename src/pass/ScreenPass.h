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
#include "BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class ScreenPass : public BasePass {
    struct InstanceData {
        alignas(8) glm::vec2 iOffset;
        alignas(8) glm::vec2 iSize;
        alignas(4) int iTextureIndex = 0;
    };

    struct LocalInfo {
        alignas(8) glm::vec2 screenSize;
    };

public:
    explicit ScreenPass(HyperGpu::GpuDevice* gpuDevice);
	~ScreenPass() override;
    void AddScreenTexture(HyperGpu::Image2D* screenTexture, const Offset2D& screenPos);
    void ClearScreenTexture();
    void Draw(HyperGpu::GpuCmd *pCmd) override;
    void SetScreenSize(const Size &size);

private:
    std::vector<InstanceData> m_vecInstanceData;
    HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
    LocalInfo m_localInfo;
    bool m_isLocalInfoDirty = true;
};

USING_RENDER_NAMESPACE_END

#endif //SCREENPASS_H