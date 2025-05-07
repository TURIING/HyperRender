/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 14:39
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef ROUNDCORNERPASS_H
#define ROUNDCORNERPASS_H

#include "../../common/common.h"
#include "../BasePass.h"
#include "HyperGpu.h"

USING_RENDER_NAMESPACE_BEGIN

class RoundCornerPass final : public BasePass {
    struct Vertex {
        glm::vec2 aPos;
    };

    struct LocalInfo {
        alignas(16) glm::vec2 resolution;
    };

public:
    explicit RoundCornerPass(HyperGpu::GpuDevice* pGpuDevice);
    ~RoundCornerPass() override;
    void SetScreenTexture(IDrawUnit* screenTexture);
    void UpdateSize(const HyperRender::Size& size) override;

private:
    HyperGpu::Buffer*   m_pVertexBuffer = nullptr;
    HyperGpu::Buffer*   m_pIndexBuffer  = nullptr;

private:
    HyperGpu::Buffer*   m_pLocalBuffer  = nullptr;
    LocalInfo           m_localInfo{};
    std::vector<Vertex> m_vertexData = {
        // 位置
        {{-1.0f, -1.0f}}, // 左下角
        {{1.0f, -1.0f}},  // 右下角
        {{1.0f, 1.0f}},   // 右上角
        {{-1.0, 1.0f}},   // 左上角
    };
    std::vector<uint32_t> m_indices = {
        0, 1, 2, // 第一个三角形（左下 - 右下 - 右上）
        2, 3, 0  // 第二个三角形（右上 - 左上 - 左下）
    };
};

USING_RENDER_NAMESPACE_END

#endif //ROUNDCORNERPASS_H
