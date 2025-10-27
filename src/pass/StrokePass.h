//
// Created by TURIING on 2025/10/27.
//

#ifndef STROKEPASS_H
#define STROKEPASS_H

#include "../common/common.h"
#include "BasePass.h"

USING_RENDER_NAMESPACE_BEGIN

class StrokePass: public BasePass {
	struct LocalInfo {
		int numStrokes;
		int width;
		int height;
		float aa = 1.5;
		glm::vec4 strokeColor; // r,g,b,a
	};

public:
	struct Circle {
		glm::vec2 center;
		float radius;
	};

	struct Stroke {
		int numCircles;
		int startIndex; // circles 数组起始位置
	};

	explicit StrokePass(HyperGpu::GpuDevice* pDevice);
	void SetOutputImage(HyperGpu::Image2D* image);
	void SetStrokeInfo(const std::vector<Stroke> &vecStroke, const std::vector<Circle> &vecCircle);

private:
	LocalInfo m_localInfo;
	HyperGpu::Buffer* m_pLocalInfoBuffer = nullptr;
	bool m_dirty = true;
	HyperGpu::Buffer* m_pCircleBuffer = nullptr;
	HyperGpu::Buffer* m_pStrokeBuffer = nullptr;
};

USING_RENDER_NAMESPACE_END

#endif //STROKEPASS_H
