/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/1 22:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "EffectTool.h"
#include "../pass/effect/RoundCornerPass.h"
#include "../pass/effect/DualKawaseBlurDownSamplePass.h"
#include "../pass/effect/DualKawaseBlurUpSamplePass.h"
#include "../pass/effect/GaussianBlurPass.h"
#include "../pass/StrokePass.h"
#include "../common/GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {
    // m_pRoundCornerPass = new RoundCornerPass(m_pGpuDevice);
    m_pDualKawaseBlurUpSamplePass = new DualKawaseBlurUpSamplePass(m_pGpuDevice);
    m_pDualKawaseBlurDownSamplePass = new DualKawaseBlurDownSamplePass(m_pGpuDevice);
    m_pGaussianBlurPass = new GaussianBlurPass(pGpuDevice);
	m_pStrokePass = new StrokePass(pGpuDevice);
}

EffectTool::~EffectTool() {
    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    // m_pRoundCornerPass->SubRef();
    m_pDualKawaseBlurUpSamplePass->SubRef();
    m_pDualKawaseBlurDownSamplePass->SubRef();
    m_pGaussianBlurPass->SubRef();
	m_pStrokePass->SubRef();
}

void EffectTool::SetRoundCorner(float radius) {
}

void EffectTool::Begin(const Area &renderArea) {
    m_renderArea = renderArea;
    begin();
	m_pCmd->BeginDebugUtilsLabel("EffectTool::Begin");
}

void EffectTool::DoDualKawaseBlur(int interation, const Offset2D &offset) {
	m_pCmd->BeginDebugUtilsLabel("EffectTool::DoDualKawaseBlur");
    m_pDualKawaseBlurUpSamplePass->SetOffset(offset);
    m_pDualKawaseBlurDownSamplePass->SetOffset(offset);

    std::vector<HyperGpu::Image2D*> vecImg;
    vecImg.push_back(m_pResultUnit->GetImage());
    const auto targetSize = m_pTargetUnit->GetSize();
    for (auto i = 1; i < interation + 1; i++) {
        auto name = std::format("EffectToolDkBlur{}", i);
        auto pImage = GpuHelper::CreateImage(m_pGpuDevice, targetSize/pow(2,i), m_pCommonSampler, name.c_str());
        vecImg.push_back(pImage);
    }

    GpuHelper::CopyImage(m_pGpuDevice, m_pCmd, m_pTargetUnit->GetImage(), vecImg[0]);

    for (auto i = 0; i < interation; i++) {
        auto image = vecImg[i+1];
        const auto imageSize = image->GetSize();
        m_pDualKawaseBlurDownSamplePass->SetTargetTexture(vecImg[i]);
        m_pDualKawaseBlurDownSamplePass->SetScreenSize(std::bit_cast<Size>(imageSize));
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pDualKawaseBlurDownSamplePass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, imageSize},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, &image},
        };
        m_pCmd->BeginRenderPass(beginInfo);
        m_pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        m_pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurDownSamplePass->Draw(m_pCmd, i);
        m_pCmd->EndRenderPass();
    }

    for (auto i = interation; i > 0; i--) {
        auto image = vecImg[i-1];
        const auto imageSize = image->GetSize();
        m_pDualKawaseBlurUpSamplePass->SetTargetTexture(vecImg[i]);
        m_pDualKawaseBlurUpSamplePass->SetScreenSize(std::bit_cast<Size>(imageSize));
        HyperGpu::BeginRenderInfo beginInfo {
            .pPipeline = m_pDualKawaseBlurUpSamplePass->GetPipeline(),
            .clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
            .renderArea = { {0, 0}, imageSize},
            .renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
            .renderAttachment = {1, &image},
        };
        m_pCmd->BeginRenderPass(beginInfo);
        m_pCmd->SetViewport({0, 0, (float)imageSize.width, (float)imageSize.height});
        m_pCmd->SetScissor({0, 0, imageSize.width, imageSize.height});
        m_pDualKawaseBlurUpSamplePass->Draw(m_pCmd, interation - i);
        m_pCmd->EndRenderPass();
    }
	m_pCmd->EndDebugUtilsLabel();
}

void EffectTool::DoGaussianBlur() {
	m_pCmd->BeginDebugUtilsLabel("EffectTool::DoGaussianBlur");
	HyperGpu::Image2D::Image2DCreateInfo inputCreateInfo;
	inputCreateInfo.aspect	 = HyperGpu::ImageAspectFlags::Color;
	inputCreateInfo.usage	 = HyperGpu::ImageUsageFlags::STORAGE | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
	inputCreateInfo.size	 = std::bit_cast<HyperGpu::Size>(m_pTargetUnit->GetSize());
	inputCreateInfo.format	 = HyperGpu::PixelFormat::R8G8B8A8;
	inputCreateInfo.pSampler = m_pCommonSampler;
	inputCreateInfo.objName	 = "GaussianBlurInputImage";
	auto inputImage			 = m_pGpuDevice->GetResourceManager()->CreateImage2D(inputCreateInfo);

	HyperGpu::Image2D::Image2DCreateInfo outputCreateInfo;
	outputCreateInfo.aspect	  = HyperGpu::ImageAspectFlags::Color;
	outputCreateInfo.usage	  = HyperGpu::ImageUsageFlags::STORAGE | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
	outputCreateInfo.size	  = std::bit_cast<HyperGpu::Size>(m_pTargetUnit->GetSize());
	outputCreateInfo.format	  = HyperGpu::PixelFormat::R8G8B8A8;
	outputCreateInfo.pSampler = m_pCommonSampler;
	outputCreateInfo.objName  = "GaussianBlurOutputImage";
	auto outputImage		  = m_pGpuDevice->GetResourceManager()->CreateImage2D(outputCreateInfo);

	GpuHelper::CopyImage(m_pGpuDevice, m_pCmd, m_pTargetUnit->GetImage(), inputImage);

	m_pGaussianBlurPass->SetTargetImage(inputImage);
	m_pGaussianBlurPass->SetOutputImage(outputImage);

	uint32_t groupX = (m_pTargetUnit->GetImage()->GetSize().width + 15) / 16;
	uint32_t groupY = (m_pTargetUnit->GetImage()->GetSize().height + 15) / 16;
	m_pGaussianBlurPass->Dispatch(m_pCmd, groupX, groupY, 1);

	GpuHelper::CopyImage(m_pGpuDevice, m_pCmd, outputImage, m_pResultUnit->GetImage());
	m_pCmd->EndDebugUtilsLabel();
}

void EffectTool::DoStroke() {
	HyperGpu::Image2D::Image2DCreateInfo outputCreateInfo;
	outputCreateInfo.aspect	  = HyperGpu::ImageAspectFlags::Color;
	outputCreateInfo.usage	  = HyperGpu::ImageUsageFlags::STORAGE | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
	outputCreateInfo.size	  = std::bit_cast<HyperGpu::Size>(m_pTargetUnit->GetSize());
	outputCreateInfo.format	  = HyperGpu::PixelFormat::R8G8B8A8;
	outputCreateInfo.pSampler = m_pCommonSampler;
	outputCreateInfo.objName  = "StrokeOutputImage";
	auto outputImage		  = m_pGpuDevice->GetResourceManager()->CreateImage2D(outputCreateInfo);

	m_pStrokePass->SetOutputImage(outputImage);

	uint32_t groupX = (outputImage->GetSize().width + 15) / 16;
	uint32_t groupY = (outputImage->GetSize().height + 15) / 16;
	m_pStrokePass->Dispatch(m_pCmd, groupX, groupY, 1);

	GpuHelper::CopyImage(m_pGpuDevice, m_pCmd, outputImage, m_pResultUnit->GetImage());
}

void EffectTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = dynamic_cast<DrawUnit *>(pTargetUnit);

    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    m_pResultUnit = BaseTool::CreateDrawUnit(m_pTargetUnit->GetArea(), "EffectToolResultUnit");
}

void EffectTool::End() {
	m_pCmd->EndDebugUtilsLabel();
    end();
}

void EffectTool::RenderToUnit(IDrawUnit *resultUnit) {
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END