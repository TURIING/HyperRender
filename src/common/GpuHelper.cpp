/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/6/14 13:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "GpuHelper.h"

USING_RENDER_NAMESPACE_BEGIN

HyperGpu::Buffer* HyperRender::GpuHelper::CreateUniformBuffer(HyperGpu::GpuDevice* pDevice, uint32_t size) {
	return pDevice->GetResourceManager()->CreateBuffer({
		.bufferType = HyperGpu::Buffer::Uniform,
		.bufferSize = size,
	});
}

HyperGpu::Buffer* GpuHelper::CreateShaderStorageBuffer(HyperGpu::GpuDevice* pDevice, uint32_t size) {
	return pDevice->GetResourceManager()->CreateBuffer({
		.bufferType = HyperGpu::Buffer::ShaderStorage,
		.bufferSize = size,
	});
}

HyperGpu::Image2D* GpuHelper::CreateImage(HyperGpu::GpuDevice *pDevice, const Size &size, HyperGpu::Sampler *pSampler, const char *name, HyperGpu::PixelFormat format) {
    HyperGpu::Image2D::Image2DCreateInfo createInfo;
    createInfo.size = std::bit_cast<HyperGpu::Size>(size);
    createInfo.pSampler = pSampler;
    createInfo.objName = name;
    createInfo.format = format;
    createInfo.usage = HyperGpu::ImageUsageFlags::SAMPLED | HyperGpu::ImageUsageFlags::COLOR_ATTACHMENT | HyperGpu::ImageUsageFlags::TRANS_DST | HyperGpu::ImageUsageFlags::TRANS_SRC;
    createInfo.aspect = HyperGpu::ImageAspectFlags::Color;
    return pDevice->GetResourceManager()->CreateImage2D(createInfo);
}

void GpuHelper::CopyImage(HyperGpu::GpuDevice* pDevice, HyperGpu::GpuCmd* pCmd, HyperGpu::Image2D *pSrc, HyperGpu::Image2D *pDst, const Offset2D &srcOffset, const Offset2D &dstOffset) {
    const auto srcSize = pSrc->GetSize();
    const HyperGpu::Area srcArea {
        std::bit_cast<HyperGpu::Offset2D>(srcOffset),
        std::bit_cast<HyperGpu::Size>(srcSize),
    };

    const HyperGpu::Area dstArea {
        std::bit_cast<HyperGpu::Offset2D>(dstOffset),
        std::bit_cast<HyperGpu::Size>(srcSize),
    };

    std::vector<HyperGpu::ImageCopyRange> ranges = {
        {
            .srcArea = srcArea,
            .dstArea = dstArea
        },
    };

    if (pCmd) {
        pCmd->CopyImage(pSrc, pDst, ranges.data(), ranges.size());
    }
    else {
        pDevice->GetCmdManager()->WithSingleCmdBuffer([&](HyperGpu::GpuCmd* pCmd) {
            pCmd->CopyImage(pSrc, pDst, ranges.data(), ranges.size());
        });
    }
}

glm::mat4 GpuHelper::GetModelMatrix(const Transform &transform) {
    auto model = glm::mat4(1.0f);
    // 平移到目标位置
    model = glm::translate(model, glm::vec3(transform.translate.x, transform.translate.y, 0.0f));
    // 进行缩放
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, 1.0f));
    // 平移到目标中心
    model = glm::translate(model, glm::vec3(transform.center.x, transform.center.y, 0.0f));
    // 进行旋转
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0, 0.0, 1.0));
    // 平移回目标左上角
    model = glm::translate(model, glm::vec3(-transform.center.x, -transform.center.y, 0.0f));
    return model;
}

USING_RENDER_NAMESPACE_END
