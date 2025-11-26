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
#include "../pass/effect/LiquifyMainPass.h"
#include "../pass/effect/LiquifyImagePass.h"
#include "../pass/StrokePass.h"
#include "../common/GpuHelper.h"
#include <fstream>
#include <sstream>


USING_RENDER_NAMESPACE_BEGIN
std::vector<StrokePass::Circle> gVecCircle;

EffectTool::EffectTool(HyperGpu::GpuDevice* pGpuDevice): BaseTool(pGpuDevice) {
    // m_pRoundCornerPass = new RoundCornerPass(m_pGpuDevice);
    m_pDualKawaseBlurUpSamplePass = new DualKawaseBlurUpSamplePass(m_pGpuDevice);
    m_pDualKawaseBlurDownSamplePass = new DualKawaseBlurDownSamplePass(m_pGpuDevice);
    // m_pGaussianBlurPass = new GaussianBlurPass(pGpuDevice);
	m_pStrokePass = new StrokePass(pGpuDevice);
	m_pLiquifyMainPass = new LiquifyMainPass(pGpuDevice);
	m_pLiquifyImagePass = new LiquifyImagePass(pGpuDevice);
	gVecCircle.reserve(100*100*10);
	std::ifstream in("data.txt");
	std::string line;
	while (std::getline(in, line)) {
		std::stringstream ss(line);
		int a, b;
		char comma;
		if (ss >> a >> comma >> b && comma == ',') {
			gVecCircle.emplace_back(glm::vec2(a,b), 20.0);
		}
	}
	in.close();
}

EffectTool::~EffectTool() {
    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    // m_pRoundCornerPass->SubRef();
    m_pDualKawaseBlurUpSamplePass->SubRef();
    m_pDualKawaseBlurDownSamplePass->SubRef();
    m_pGaussianBlurPass->SubRef();
	m_pLiquifyMainPass->SubRef();
	m_pLiquifyImagePass->SubRef();
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
	inputCreateInfo.format	 = HyperGpu::PixelFormat::R8G8B8A8_SRGB;
	inputCreateInfo.pSampler = m_pCommonSampler;
	inputCreateInfo.objName	 = "GaussianBlurInputImage";
	auto inputImage			 = m_pGpuDevice->GetResourceManager()->CreateImage2D(inputCreateInfo);

	HyperGpu::Image2D::Image2DCreateInfo outputCreateInfo;
	outputCreateInfo.aspect	  = HyperGpu::ImageAspectFlags::Color;
	outputCreateInfo.usage	  = HyperGpu::ImageUsageFlags::STORAGE | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
	outputCreateInfo.size	  = std::bit_cast<HyperGpu::Size>(m_pTargetUnit->GetSize());
	outputCreateInfo.format	  = HyperGpu::PixelFormat::R8G8B8A8_SRGB;
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

void EffectTool::DoLiquify(const PointI &newMousePos, const PointI &oldMousePos, bool isPressed, bool firstFrame) {
	const auto size = m_pTargetUnit->GetSize();
	m_pCmd->BeginDebugUtilsLabel("EffectTool::DoLiquify");
	if (m_vecLiquifyImage.empty()) {
		for (auto i = 0; i < 2; i++) {
			m_vecLiquifyImage.push_back(GpuHelper::CreateImage(m_pGpuDevice, size, m_pCommonSampler));
			m_pCmd->ClearColorImage(m_vecLiquifyImage[i], {0, 0, 0, 0});
		}
	}

	{
		m_pLiquifyMainPass->SetLocalInfo(size, newMousePos, oldMousePos, isPressed, firstFrame);
		m_pLiquifyMainPass->SetUvInputImage(m_vecLiquifyImage[0]);
		HyperGpu::BeginRenderInfo beginInfo {
			.pPipeline = m_pLiquifyMainPass->GetPipeline(),
			.clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
			.renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
			.renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
			.renderAttachment = {1, &m_vecLiquifyImage[1]},
		};
		m_pCmd->BeginRenderPass(beginInfo);
		m_pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
		m_pCmd->SetScissor({0, 0, size.width, size.height});
		m_pLiquifyMainPass->Draw(m_pCmd);
		m_pCmd->EndRenderPass();
	}

	{
		m_pLiquifyImagePass->SetTargetImage(m_pTargetUnit->GetImage());
		m_pLiquifyImagePass->SetUvInputImage(m_vecLiquifyImage[1]);
		HyperGpu::BeginRenderInfo beginInfo {
			.pPipeline = m_pLiquifyImagePass->GetPipeline(),
			.clearValue = { { HyperGpu::AttachmentType::COLOR, {0.0, 0.0, 0.0, 0.0 }}},
			.renderArea = { {0, 0}, std::bit_cast<HyperGpu::Size>(size)},
			.renderAttachmentType = HyperGpu::RenderAttachmentType::Image2D,
			.renderAttachment = {1, (HyperGpu::Image2D*[]){m_pResultUnit->GetImage()}},
		};
		m_pCmd->BeginRenderPass(beginInfo);
		m_pCmd->SetViewport({0, 0, (float)size.width, (float)size.height});
		m_pCmd->SetScissor({0, 0, size.width, size.height});
		m_pLiquifyImagePass->Draw(m_pCmd);
		m_pCmd->EndRenderPass();
	}

	std::swap(m_vecLiquifyImage[0], m_vecLiquifyImage[1]);
	m_pCmd->EndDebugUtilsLabel();
}

void EffectTool::DoStroke() {
	m_pCmd->BeginDebugUtilsLabel("EffectTool::DoStroke");

	LOG_INFO("circle count {}", gVecCircle.size());
	std::vector<StrokePass::Stroke> strokes;
	for (auto i = 0; i < gVecCircle.size()/200; ++i) {
		strokes.push_back({200, 200*i});
		// if (i == 30) break;
	}

	m_pStrokePass->SetStrokeInfo(strokes, gVecCircle);

	uint32_t groupX = (m_pOutputImage->GetSize().width + 15) / 16;
	uint32_t groupY = (m_pOutputImage->GetSize().height + 15) / 16;
	m_pStrokePass->Dispatch(m_pCmd, groupX, groupY, 1);

	m_pCmd->EndDebugUtilsLabel();
}

void EffectTool::SetTargetUnit(IDrawUnit *pTargetUnit) {
    m_pTargetUnit = dynamic_cast<DrawUnit *>(pTargetUnit);

    if (m_pResultUnit) {
        m_pResultUnit->SubRef();
    }
    m_pResultUnit = BaseTool::CreateDrawUnit(m_pTargetUnit->GetArea(), "EffectToolResultUnit");

	HyperGpu::Image2D::Image2DCreateInfo outputCreateInfo;
	outputCreateInfo.aspect	  = HyperGpu::ImageAspectFlags::Color;
	outputCreateInfo.usage	  = HyperGpu::ImageUsageFlags::STORAGE | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
	outputCreateInfo.size	  = std::bit_cast<HyperGpu::Size>(m_pTargetUnit->GetSize());
	outputCreateInfo.format	  = HyperGpu::PixelFormat::R8G8B8A8_SRGB;
	outputCreateInfo.pSampler = m_pCommonSampler;
	outputCreateInfo.objName  = "OutputImage";
	m_pOutputImage		  = m_pGpuDevice->GetResourceManager()->CreateImage2D(outputCreateInfo);

	m_pStrokePass->SetOutputImage(m_pOutputImage);
}

void EffectTool::End() {
	m_pCmd->EndDebugUtilsLabel();
    end();
}

void EffectTool::RenderToUnit(IDrawUnit *resultUnit) {
	// GpuHelper::CopyImage(m_pGpuDevice, nullptr, m_pOutputImage, m_pResultUnit->GetImage());
    BaseTool::CopyDrawUnit(m_pResultUnit, resultUnit);
}

USING_RENDER_NAMESPACE_END