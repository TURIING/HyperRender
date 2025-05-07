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

namespace HyperGpu
{
    class Pipeline;
    class GpuDevice;
    class Buffer;
    class Image2D;
}

using namespace HyperGpu;

USING_RENDER_NAMESPACE_BEGIN

class ScreenPass : public BasePass {
    struct Vertex {
        glm::vec2 aPos;
        glm::vec2 aTexCoord;
    };

public:
    explicit ScreenPass(GpuDevice* gpuDevice);
	~ScreenPass() override;
    void                    SetScreenTexture(Image2D* screenTexture);

private:
    std::vector<Vertex> m_vertexData = {
        // 位置             // 纹理坐标
        {{-1.0f, -1.0f}, {0.0f, 0.0f}}, // 左下角
        {{1.0f, -1.0f}, {1.0f, 0.0f}},  // 右下角
        {{1.0f, 1.0f}, {1.0f, 1.0f}},   // 右上角
        {{-1.0, 1.0f}, {0.0f, 1.0f}},   // 左上角
    };
    std::vector<uint32_t> m_indices = {
        0, 1, 2, // 第一个三角形（左下 - 右下 - 右上）
        2, 3, 0  // 第二个三角形（右上 - 左上 - 左下）
    };
};

USING_RENDER_NAMESPACE_END

#endif //SCREENPASS_H
