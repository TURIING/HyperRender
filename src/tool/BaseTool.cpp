/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/6 15:29
* @version: 1.0
* @description: 
********************************************************************************/
#include "BaseTool.h"
#include "BaseTool.h"
#include "../common/Camera.h"
#include "../common/GpuHelper.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

USING_RENDER_NAMESPACE_BEGIN
BaseTool::BaseTool(HyperGpu::GpuDevice* pGpuDevice) : m_pGpuDevice(pGpuDevice) {
    m_pGpuDevice->AddRef();
    m_pCmd           = m_pGpuDevice->GetCmdManager()->CreateCommandBuffer();
    m_pCommonSampler = m_pGpuDevice->GetResourceManager()->CreateSampler({});
    m_pRenderQueue   = m_pGpuDevice->CreateQueue(HyperGpu::QueueType::Graphics);
    m_pRenderFence   = m_pGpuDevice->GetSyncManager()->CreateFence();
    m_pGlobalBuffer = GpuHelper::CreateUniformBuffer(m_pGpuDevice, sizeof(GlobalInfo));
}

BaseTool::~BaseTool() {
    m_pCommonSampler->SubRef();
    m_pGlobalBuffer->SubRef();
    m_pRenderFence->SubRef();
    m_pRenderQueue->SubRef();
    m_pCmd->SubRef();
    m_pGpuDevice->SubRef();
}

void BaseTool::updateSize(const Size& size) {
    const auto halfWidth  = size.width  / 2;
    const auto halfHeight = size.height / 2;

    Camera camera;
    camera.SetOrtho(0, size.width, size.height, 0, -1.0f, 1000.0f);

    m_globalInfo.view = camera.GetViewMatrix();
    m_globalInfo.proj = camera.GetProjectionMatrix();

    m_pGlobalBuffer->WriteData(reinterpret_cast<uint8_t*>(&m_globalInfo), sizeof(GlobalInfo));
}

void BaseTool::ClearColor(IDrawUnit* targetUnit, Color color) {
	const auto unit = dynamic_cast<DrawUnit*>(targetUnit);

	m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
		pCmd->ClearColorImage(unit->GetImage(), std::bit_cast<HyperGpu::Color>(color));
    });
}

DrawUnit* BaseTool::CreateDrawUnit(const Area& area, const char* name) {
    return new DrawUnit(m_pGpuDevice, {area, m_pCommonSampler, name});
}

void BaseTool::CopyDrawUnit(IDrawUnit *pSrcUnit, IDrawUnit *pDstUnit) {
    LOG_ASSERT(pSrcUnit->GetTextureSize() == pDstUnit->GetTextureSize());
    const auto srcUnit = dynamic_cast<DrawUnit*>(pSrcUnit);
    const auto dstUnit = dynamic_cast<DrawUnit*>(pDstUnit);

    m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
        HyperGpu::Area area {
            {0, 0},
            std::bit_cast<HyperGpu::Size>(pSrcUnit->GetTextureSize())
        };
        std::vector<HyperGpu::ImageCopyRange> ranges = {
            {
                .srcArea = area,
                .dstArea = area
            },
        };
        pCmd->CopyImage(srcUnit->GetImage(), dstUnit->GetImage(), ranges.data(), ranges.size());
    });
}

void BaseTool::FillDrawUnit(IDrawUnit *pUnit, const void *data, uint64_t size, const Offset2D &offset) {
    LOG_ASSERT(pUnit);

    const auto unit = dynamic_cast<DrawUnit*>(pUnit);
    m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
        const auto image = unit->GetImage();
        pCmd->CopyBufferToImage(
            image,
            data,
            size,
            {
                std::bit_cast<HyperGpu::Offset2D>(offset),
                std::bit_cast<HyperGpu::Size>(unit->GetSize())
            }
        );
    });
}

void BaseTool::SaveDrawUnit(IDrawUnit *pUnit, const char *fileName) {
    LOG_ASSERT(pUnit && fileName);
    const auto unit = dynamic_cast<DrawUnit*>(pUnit);
    SaveImage(unit->GetImage(), fileName);
}

void BaseTool::SaveImage(HyperGpu::Image2D *pImage, const std::string &fileName) const {
    const auto [width, height] = pImage->GetSize();
    const auto pixelFormatSize = gPixelFormatToSizeByte[TO_I32(pImage->GetPixelFormat())];
    const auto pixelFormatChannelCount = gPixelFormatToChannelCount[TO_I32(pImage->GetPixelFormat())];
    const auto bufferSize = pixelFormatSize * width * height;

    auto stageBuffer = m_pGpuDevice->GetResourceManager()->CreateBuffer( {
        .bufferType = HyperGpu::Buffer::TransferDst,
        .bufferSize = bufferSize,
        .data = nullptr,
    });

    m_pGpuDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
        const HyperGpu::Area area {
            .offset = { 0, 0 },
            .size = pImage->GetSize(),
        };
        pCmd->CopyImageToBuffer(pImage, stageBuffer, area);
    });

    void *pData = nullptr;
    stageBuffer->Map(0, bufferSize, &pData);

    const auto path = "/Users/turiing/Desktop/" + fileName;
    stbi_write_png(path.c_str(), width, height, pixelFormatChannelCount, pData, width * pixelFormatSize);

    stageBuffer->UnMap();
    stageBuffer->SubRef();
}

void BaseTool::begin() const {
    m_pRenderFence->Wait();
    m_pRenderFence->Reset();
    m_pCmd->Reset();
    m_pCmd->Begin();
}

void BaseTool::end(const std::vector<HyperGpu::Semaphore*>& signalSemaphore) const {
    m_pCmd->End();
    this->SubmitCmd({m_pCmd}, {}, signalSemaphore, m_pRenderFence);
    m_pRenderFence->Wait();
}

void BaseTool::SubmitCmd(const std::vector<HyperGpu::GpuCmd*>&    cmds,
                         const std::vector<HyperGpu::Semaphore*>& waitSemaphores,
                         const std::vector<HyperGpu::Semaphore*>& signalSemaphores,
                         HyperGpu::Fence*                         pFence) const {
    HyperGpu::Queue::SubmitInfo info{
        .pWaitSemaphores = waitSemaphores.data(),
        .waitSemaphoreCount = TO_U32(waitSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
        .signalSemaphoreCount = TO_U32(signalSemaphores.size()),
        .pFence = pFence,
        .pCmd = cmds.data(),
        .cmdCount = TO_U32(cmds.size())
    };
    m_pRenderQueue->Submit(info);
}

void BaseTool::clearColor(HyperGpu::GpuCmd* pCmd, IDrawUnit* targetUnit, Color color) {
    pCmd->ClearColorImage(dynamic_cast<DrawUnit*>(targetUnit)->GetImage(), std::bit_cast<HyperGpu::Color>(color));
}

USING_RENDER_NAMESPACE_END
